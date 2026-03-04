#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void second_pass(file_state *am_file, symbol *head)
{
    char line[MAX_LINE_LENGTH];
    char *args[MAX_ARG_LENGTH];
    symbol *head, *curr;
    head = NULL;
    curr = NULL; 
    IC = MEM_START;


    while(fgets(line, MAX_LINE_LENGTH, am_file->ptr) != NULL)
    {
        char *arg_string;
        char *argument;
        char *label = NULL;
        int argc;
        am_file->current_line++;

        if(is_empty_line(line) || is_comment(line)) continue;

        argument = strtok(line, " \t\n");
        if (!argument) continue; /* NULL */
        if(is_label(argument)) /* skiping labels */
            argument = strtok(NULL, " \t\n");


        arg_string = strtok(NULL, "\n");
    
        argc = 0;
        if (arg_string) 
            argc = tokenize(arg_string, args);
    
    
        switch(line_type(argument)) /* type of sentance */
        {
            case data: /* directive */
                continue;

            case external: /* instruction */
                continue;

            case code:
            

            case entry:
            {
                symbol *temp;
                if(!label_exist(args[0], head)) error(am_file, "Label not found");
                temp = get_symbol(args[0], head);
                temp->atr = entry;
                break;
            }
        }

    }
    /* finished file */



}