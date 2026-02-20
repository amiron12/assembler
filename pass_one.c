#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



static attribute str_type(char *str);

void start(FILE *fp)
{
    char line[MAX_LINE_LENGTH];
    char *args[MAX_ARG_LENGTH];
    symbol *head, *curr;
    int line_count = 0;
    head = NULL;
    curr = NULL; 

    while(fgets(line, MAX_LINE_LENGTH, fp) != NULL)
    {
        int argc, index;
        char *label = NULL;
        index = 0;
        argc = tokenize(line, args); /* saves all words in args, removing leading and trailing whitespaces, and trailing commas*/
        line_count++;
        if(!argc || **args==';') continue;


            if(is_label(args[index]) && !label_exist(args[index], head))
            {
                if(index){} /* TODO: syntax error */
                label = clean_label(args[index]);
                if(!not_reserved(args[index]))
                {

                    /* TODO: error */
                }
                index++; /* TODO: check not over argc */

            }


            switch (str_type(args[index]))
            {
            case (DATA):
                add_symbol(label, &curr, DC, DATA);
                if(is_data(args[index]))
                    DC+=(argc-(index+1));
                else if(is_string(args[index]))
                    DC+=(2+(strlen(args[index+1])));
                break;
            
            case (CODE):
                add_symbol(label, &curr, IC, CODE);
                IC+=get_instruction_operands(args[index]);
                break;
            
            case (EXTERNAL):
                add_symbol(label, &curr, ZERO, EXTERNAL);
                break;
            
            case (ENTRY):
                add_symbol(label, &curr, ZERO, ENTRY);
                break;
                
            default:
                /* TODO: error */
                break;
            }
    
        }

}



static attribute str_type(char *str)
{
    if(is_directive(str)) return DATA;
    if(is_instruction(str)) return CODE;
    if(is_extern(str)) return EXTERNAL;
    if(is_entry(str)) return ENTRY;
    return NULL;
}






