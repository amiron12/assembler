#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void second_pass(file_state *am_file, symbol *head)
{
    char line[MAX_LINE_LENGTH];
    char *operands[MAX_ARG_LENGTH];
    symbol *head, *curr;
    head = NULL;
    curr = NULL; 
    IC = MEM_START;


    while(fgets(line, MAX_LINE_LENGTH, am_file->ptr) != NULL)
    {
        char *arg_string;
        char *argument;
        char *label = NULL;
        int op_count;
        am_file->current_line++;

        if(is_empty_line(line) || is_comment(line)) continue;

        argument = strtok(line, " \t\n");
        if (!argument) continue; /* NULL */
        if(is_label(argument)) /* skiping labels */
            argument = strtok(NULL, " \t\n");


        arg_string = strtok(NULL, "\n");
    
        op_count = 0;
        if (arg_string) 
            op_count = tokenize(arg_string, operands);
    
    
        switch(sentence_type(argument)) /* type of sentance */
        {
            case directive: /* directive */
            {
                if(is_entry(argument))
                {
                    symbol *temp;
                    if(!label_exist(operands[0], head)) error(am_file, "Label not found");
                    temp = get_symbol(operands[0], head);
                    temp->atr = entry;
                    break;
                }
            
                continue;
            }

            case instruction: /* instruction */
            {
                attribute att;
                int i, mode;
                op_mode m;
                IC++;
                if(op_count != get_instruction_operands(argument)) error( am_file ,"Invalid number of operands");
                while(i<op_count)
                {
                    m = get_mode(operands[i]);
                    if(m == IMM || m == REG)
                        IC++;

                    if(m == DIR && code_image[IC].type == UNKNOWN)
                    {
                        symbol *temp;
                        if(!label_exist(operands[i], head)) error(am_file, "Label not found");
                        temp = get_symbol(operands[i], head);
                        if(temp->atr == external)
                        {
                            code_image[IC].word = ZERO;
                            code_image[IC].type = EXTERNAL;
                            /* TODO: record the ic and label name for the ext file */
                        }

                    }

                }
            }

           
        }

    }
    /* finished file */



}