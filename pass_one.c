#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



/*
    Invalid operation names.
    ◦ Incorrect number of operands or invalid addressing modes for a specific operation.
    ◦ Invalid label names or duplicate

*/

static attribute line_type(char *str);

void start(file_state *fs)
{

    FILE *fp = fs->ptr;

    char line[MAX_LINE_LENGTH];
    char *args[MAX_ARG_LENGTH];
    int label_flag;
    char *curr_arg;
    symbol *head, *curr;
    head = NULL;
    curr = NULL; 


    while(fgets(line, MAX_LINE_LENGTH, fp) != NULL)
    {
        int argc;
        char *rest;
        char *label = NULL;
        label_flag = FALSE;
        fs->current_line++;

        if(is_empty_line(line) || is_comment(line)) continue;

        curr_arg = strtok(line, " \t");
        rest = strtok(NULL, "");

        if(is_label(curr_arg) && !label_exist(curr_arg, head))
        {
            label = curr_arg;
            clean_string(&label);
            if(!not_reserved(label)) {}
            label_flag = TRUE;
            curr_arg = strtok(rest, " \t");
            rest = strtok(NULL, "");
        }

        while (*rest == ' ' || *rest == '\t') 
           rest++;

        argc = tokenize(rest, args); 
        if(argc == NEG)
        {
            error(fs, "Tokenization error");
            continue; /* TODO: change */
        }

        switch(line_type(curr_arg)) /* type of sentance */
        {
            case data: 
            {
                if(label_flag) add_symbol(label, &curr, &head, DC, data);
                
                if(is_data(curr_arg) && validate_data(args))
                    encode_data(args);        
                else if(is_string(curr_arg) && validate_string(*args))
                    encode_string(*args);
                break;
            }

            case code:
            {
                int src, dest, i;
                i=0;
                if(label_flag) add_symbol(label, &curr,&head, IC, code);
                if(argc != get_instruction_operands(curr_arg)) error( fs ,"Invalid number of operands");
                src = (i<argc)?get_mode(args[i++]):ZERO; 
                dest = (i<argc)?get_mode(args[i]):ZERO;
                encode_instruction(curr_arg, src, dest);
                i = 0;
                while (i<argc)
                    encode_operand(args[i++]);
                break;
            }

            case external:
            {
                int i,len;
                if(argc!=1) error(fs, "Extraneous text after external value");
                if(!isalpha(*curr_arg)) error(fs, "Invalid label name"); /* first character is non alphabetical */
                len = strlen(curr_arg);
                if((len--)>LABEL_LENGTH) error(fs, "Label name is too long");
                for(i=1;i<len;i++)
                    if(!isalnum(curr_arg[i])) error(fs, "Invalid label name");
                if(label_exist(curr_arg, head)) error(fs, "Label name already exists");
                if(!not_reserved(curr_arg)) error(fs, "Label name cannot be a reserved word");
                add_symbol(curr_arg, &curr, &head, 0, external);
                break;
            }

            case entry:
            break;
        }

    }
    print_symbol_table(head);/* TODO: delete */
}



static attribute line_type(char *str)
{
    if(is_directive(str)) return data;
    if(is_instruction(str)) return code;
    if(is_extern(str)) return external;
    else return entry; /* TODO: legit assumption? */
}











