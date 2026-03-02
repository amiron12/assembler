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
    symbol *head, *curr;
    head = NULL;
    curr = NULL; 


    while(fgets(line, MAX_LINE_LENGTH, fp) != NULL)
    {
        char *arg_string;
        char *token;
        char *label = NULL;
        int argc;
        label_flag = FALSE;
        fs->current_line++;

        if(is_empty_line(line) || is_comment(line)) continue;

        token = strtok(line, " \t\n");
        if (!token) continue;

        if(is_label(token)) 
        {
            label = token; 
            clean_string(&label); 
            token = strtok(NULL, " \t\n");
            label_flag = TRUE;
        }

        arg_string = strtok(NULL, "\n");
    
        argc = 0;
        if (arg_string) 
            argc = tokenize(arg_string, args);
    
        switch(line_type(token)) /* type of sentance */
        {
            case data: 
            {
                if(label_flag) add_symbol(label, &curr, &head, DC, data);
                
                if(is_data(token) && validate_data(args))
                    encode_data(args);        
                else if(is_string(token) && validate_string(*args))
                    encode_string(*args);
                break;
            }

            case code:
            {
                int src, dest, i;
                i=0;
                if(label_flag) add_symbol(label, &curr,&head, IC, code);
                if(argc != get_instruction_operands(token)) error( fs ,"Invalid number of operands");
                src = (i<argc)?get_mode(args[i++]):ZERO; 
                dest = (i<argc)?get_mode(args[i]):ZERO;
                encode_instruction(token, src, dest);
                i = 0;
                while (i<argc)
                    encode_operand(args[i++]);
                break;
            }

            case external:
            {
                int i,len;
                if(argc!=1) error(fs, "Extraneous text after external value");
                if(!isalpha(*token)) error(fs, "Invalid label name"); /* first character is non alphabetical */
                len = strlen(token);
                if((len--)>LABEL_LENGTH) error(fs, "Label name is too long");
                for(i=1;i<len;i++)
                    if(!isalnum(token[i])) error(fs, "Invalid label name");
                if(label_exist(token, head)) error(fs, "Label name already exists");
                if(!not_reserved(token)) error(fs, "Label name cannot be a reserved word");
                add_symbol(token, &curr, &head, 0, external);
                break;
            }

            case entry:
            break;
        }

    }
    
    
    /* finished file */
    
    
    
    print_symbol_table(head);/* TODO: delete */
}

void update_symbols(symbol *head)
{
    
}



static attribute line_type(char *str)
{
    if(is_directive(str)) return data;
    if(is_instruction(str)) return code;
    if(is_extern(str)) return external;
    else return entry; /* TODO: legit assumption? */
}











