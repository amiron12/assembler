#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void second_pass(file_state *am_file, symbol *head)
{
    char line[MAX_LINE_LENGTH];
    char *operands[MAX_ARG_LENGTH];
    IC = MEM_START;
    rewind(am_file->ptr);
    am_file->current_line = ZERO;
    
    while (fgets(line, MAX_LINE_LENGTH, am_file->ptr) != NULL)
    {
        char *arg_string;
        char *argument;
        int op_count;
        am_file->current_line++;

        if (is_empty_line(line) || is_comment(line)) continue;

        argument = strtok(line, " \t\n");
        if (!argument) continue;           /* NULL */
        
        if (is_label(argument)) /* skiping labels */
            argument = strtok(NULL, " \t\n");

        arg_string = strtok(NULL, "\n");

        op_count = 0;
        if (arg_string)
            op_count = tokenize(arg_string, operands);

        if (is_data(argument) || is_string(argument) || is_extern(argument))
            continue;

        if (is_entry(argument))
        {
            symbol *temp;
            if (!label_exist(operands[0], head))
                error(am_file, "Label not found");
            temp = get_symbol(operands[0], head);
            temp->atr = entry; /* TODO: add the attribute or change it? */
        }

        else
        {
            int i;
            IC++;
            i = 0;
            if (op_count != get_instruction_operands(argument))
                error(am_file, "Invalid number of operands");

            while (i < op_count) /* parsing through the operands */
            {
                if(code_image[ICINDEX].type == UNKNOWN) /* address not set */
                {    
                    if(label_exist(operands[i], head))
                    {
                        symbol *temp = get_symbol(operands[i], head);
                        if (temp->atr == external)
                        {
                            code_image[ICINDEX].word = ZERO;
                            code_image[ICINDEX].type = EXTERNAL;
                        }
                        else
                        {
                            code_image[ICINDEX].word = temp->address; 
                            code_image[ICINDEX].type = RELOCATABLE;
                        }
                    }
                    else
                        error(am_file, "Label not found");
                }

                if(is_relative(operands[i]))
                {
                    clean_string(&operands[i]);
                    if(label_exist(operands[i], head))
                    {
                        symbol *temp;
                        int address;
                        temp = get_symbol(operands[i], head);
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

    save_symbol_table(head, "second_pass.txt");
    save_machine_images("second_pass.txt");
}

