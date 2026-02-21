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
    int in_label;
    char *curr_arg;
    symbol *head, *curr;
    head = NULL;
    curr = NULL; 


    while(fgets(line, MAX_LINE_LENGTH, fp) != NULL)
    {
        int argc, index;
        char *rest;
        char *label = NULL;
        in_label = FALSE;
        index = 0;
        fs->current_line++;
        
        if(!argc || **args==';') continue;

        curr_arg = strtok(line, " \t");
        rest = strtok(NULL, "\n");

        if(is_label(curr_arg) && !label_exist(curr_arg, head))
        {
            label = clean_label(curr_arg);
            if(!not_reserved(label)) {}
            in_label = TRUE;
            curr_arg = strtok(rest, " \t");
            rest = strtok(NULL, "\n");
        }

        argc = tokenize(rest, args); 
        
            
    
        }

}








static void instruction(char *inst, char *args[], int argc, file_state *state)
{
    int src, dest;
    int i = 0;
    if(argc != get_instruction_operands(inst)) error( state ,"Invalid number of operands");
    
    /* instruction has 0 - 2 operands, this makes the address 0 if no operand */
    src = (i<=argc)?get_mode(args[i++]):ZERO; 
    dest = (i<=argc)?get_mode(args[i--]):ZERO;

    encode_instruction(inst, src, dest);
    while (i<argc)
        encode_operand(args[i++]);
}



static int data(char *args[], char *directive, int argc)
{

    if(is_data(directive))
        if(validate_data(args))
            encode_data(args);        
    if(is_string(directive))
        if(validate_string(args))
            encode_string(args);
    return OK;
}

static attribute line_type(char *str)
{
    if(is_directive(str)) return data;
    if(is_instruction(str)) return code;
    if(is_extern(str)) return external;
    if(is_entry(str)) return entry;
    return NULL;
}











