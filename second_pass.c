#include "second_pass.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "symbols.h"
#include "output.h"
#include "text_parsing.h"
#include "machine_image.h"
#include "structs.h"
#include "constants.h"
#include "utils.h"



void second_pass(file_data *am_file)
{
    char buffer[LINE_LENGTH];
    IC = MEM_START;
    rewind(am_file->ptr);
    am_file->current_line = ZERO;
    init_output_files(am_file->name);
    
    fprintf(stderr,"[INFO] starting second pass\n");
    
    
    while (fgets(buffer, LINE_LENGTH, am_file->ptr) != NULL)
    {
        char *operands[MAX_ARG_LENGTH];
        char *argument, *line;
        int op_count;
        am_file->current_line++;
        line = buffer;

        get_next_word(&line, &argument);
        
        if(argument[strlen(argument)-1] == ':') /* skiping labels */
            get_next_word(&line, &argument);


        op_count = tokenize(line, operands, am_file);

        if (is_data(argument) || is_string(argument) || is_extern(argument))
            continue;

        
        if (is_entry(argument))
        {
            symbol *temp;
            if (symbol_exists(*operands, am_file->symbol_list))
            {
                temp = get_symbol(*operands, am_file->symbol_list);
                set_attribute(temp, entry);
                append_entry(temp, am_file->name);
            }
            else
                error(am_file, "Label not found");
        }

        else
        {
            int i = 0;
            IC++;
            while (i < op_count) /* parsing through the operands */
            {
                if(code_image[ICINDEX].type == UNKNOWN) /* address not set */
                {    
                    if(symbol_exists(operands[i], am_file->symbol_list))
                    {
                        symbol *temp = get_symbol(operands[i], am_file->symbol_list);
                        if (is_attribute(temp, external))
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
                    if(symbol_exists(operands[i], am_file->symbol_list))
                    {
                        symbol *temp;
                        int address;
                        temp = get_symbol(operands[i], am_file->symbol_list);
                        if(is_attribute(temp, external))
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
    
    if(am_file->error_flag)
    {
        abort_file(am_file);
        return;
    }
    create_obj_file(am_file->name, am_file->symbol_list);
}
