#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int is_label(char *str);

void start(FILE *fp)
{
    char line[MAX_LINE_LENGTH];
    char *args[MAX_ARG_LENGTH];
    symbol *head, *curr;
    head = NULL;
    curr = NULL; 
    while(fgets(line, MAX_LINE_LENGTH, fp) != NULL)
    {
        int argc, index;
        char *label = NULL;
        index = 0;
        argc = tokenize(line, args); /* saves all words in args, removing leading and trailing whitespaces, and trailing commas*/
        
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

            if(is_directive(args[index]))
            {
                add_symbol(label, &curr, DC, DATA);
                if(is_data(args[index]))
                    DC+=(argc-(index+1));
                else if(is_string(args[index]))
                    DC+=(2+(strlen(args[index+1])));
                continue;
            }
            if(is_instruction(args[index]))
            {
                add_symbol(label, &curr, IC, CODE);
                IC+=get_instruction_operands(args[index]);
                continue;
            }
            if(is_extern(args[index]))
            {
                add_symbol(label, &curr, ZERO, EXTERNAL);
                continue;
            }
            /* TODO: entry */
            

    
}
}











