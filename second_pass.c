#include "assembler.h"
#include "symbol_table.h"
#include "output.h"
#include "text_parsing.h"
#include "machine_image.h"
#include "first_pass.h"
#include "second_pass.h"
#include "const_tables.h"
#include "utils.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void second_pass(file_state *am_file, symbol *head)
{
    char line[LINE_LENGTH];
    IC = MEM_START;
    rewind(am_file->ptr);
    am_file->current_line = ZERO;
    init_output_files(am_file->name);
    
    fprintf(stderr,"[INFO] starting second pass\n");
    
    
    while (fgets(line, LINE_LENGTH, am_file->ptr) != NULL)
    {
        char *operands[MAX_ARG_LENGTH];
        char *arg_string;
        char *argument;
        int op_count;
        am_file->current_line++;

        if (is_empty_line(line) || is_comment(line)) continue;

        argument = strtok(line, " \t");
        
        if (is_label(argument)) /* skiping labels */
            argument = strtok(NULL, " \t\n");

        arg_string = strtok(NULL, "\n");

        op_count = tokenize(arg_string, operands, am_file);

        if (is_data(argument) || is_string(argument) || is_extern(argument))
            continue;

        
        if (is_entry(argument))
        {
            symbol *temp;
            if (symbol_exists(operands[0], head))
            {
                temp = get_symbol(operands[0], head);
                temp->atr = entry; /* TODO: add the attribute or change it? */
                append_entry(temp, am_file->name);
            }
            else
                {
                    error(am_file, "Label not found");
                }
        }

        else
        {
            int i = 0;
            IC++;
            while (i < op_count) /* parsing through the operands */
            {
                if(code_image[ICINDEX].type == UNKNOWN) /* address not set */
                {    
                    if(symbol_exists(operands[i], head))
                    {
                        symbol *temp = get_symbol(operands[i], head);
                        if (temp->atr == external)
                        {
                            code_image[ICINDEX].word = ZERO;
                            code_image[ICINDEX].type = EXTERNAL;
                            append_external(temp, am_file->name);
                        }
                        else
                        {
                            code_image[ICINDEX].word = temp->address; 
                            code_image[ICINDEX].type = RELOCATABLE;
                        }
                    }
                    else
                        error(am_file, "Label not found"); /* TODO: why showing for external labels */
                }

                if(is_relative(operands[i]))
                {
                    clean_string(&operands[i]);
                    if(symbol_exists(operands[i], head))
                    {
                        symbol *temp;
                        int address;
                        temp = get_symbol(operands[i], head);
                        if(temp->atr == external)
                        {
                            error(am_file, "Relative label cannot be external");
                            continue;
                        }
                        address = (temp->address)-IC;
                        code_image[ICINDEX].word = address;
                        code_image[ICINDEX].type = ABSOLUTE;

                    }
                    else
                        error(am_file, "Label not found");
                }
                IC++;
                i++;
            }   
        }

    }

    save_symbol_table(head, "test_output/second_pass.txt");
    save_machine_images("test_output/second_pass.txt");
    
    
    if(am_file->error_flag)
    {
        free_symbols(head);
        delete_output_files(am_file->name);
        return;
    }
    create_obj_file(am_file->name, head);
    free_symbols(head);
}
