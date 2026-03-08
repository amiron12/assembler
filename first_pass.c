#include "assembler.h"
#include "symbol_table.h"
#include "output.h"
#include "text_parsing.h"
#include "utils.h"
#include "const_tables.h"
#include "first_pass.h"
#include "second_pass.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



/*
    Invalid operation names.
    ◦ Incorrect number of operands or invalid addressing modes for a specific operation.
    ◦ Invalid label names or duplicate

*/
static void update_symbols(symbol *head);

void start_pass(file_state *am_file)
{
    char line[MAX_LINE_LENGTH];
    char *operands[MAX_ARG_LENGTH];
    int label_flag;
    symbol *curr, *head;
    head = NULL;
    curr = NULL; 

    while(fgets(line, MAX_LINE_LENGTH, am_file->ptr) != NULL)
    {
        char *arg_string;
        char *argument;
        char *label = NULL;
        int op_count;
        label_flag = FALSE;
        am_file->current_line++;

        if(is_empty_line(line) || is_comment(line)) continue;

        argument = strtok(line, " \t");
        if (!argument) continue;

        if(is_label(argument)) 
        {
            label = argument; 
            clean_string(&label); 
            argument = strtok(NULL, " \t\n");
            label_flag = TRUE;
        }

        arg_string = strtok(NULL, "\n");
    
        op_count = 0;
        if (arg_string) 
            op_count = tokenize(arg_string, operands);
    
        switch(sentence_type(argument)) /* type of sentance : directive or instruction */
        {
            case directive: 
            {    
                switch(dir_type(argument))
                {
                    case data:
                    {
                        if(label_flag) add_symbol(label, &curr, &head, DC, data);
                        
                        if(is_data(argument) && validate_data(operands))
                            encode_data(operands); /*its a data directive*/        
                        else if(is_string(argument) && validate_string(*operands))
                            encode_string(*operands); /*its a string directive*/
                        break;
                    }

                    case external:
                    {
                        int i,len;
                        char *label;
                        label = operands[0];
                        len = strlen(label);
                        if(op_count!=1)
                            error(am_file, "Extraneous text after external value");
                        if(!isalpha(*label)) 
                            error(am_file, "Invalid label name"); /* first character is non alphabetical */
                        if((len--)>LABEL_LENGTH) 
                            error(am_file, "Label name is too long");
                        for(i=1;i<len;i++)
                            if(!isalnum(label[i])) 
                                error(am_file, "Invalid label name");
                        if(symbol_exists(label, head)) 
                            error(am_file, "Label name already exists");
                        if(!not_reserved(label))
                             error(am_file, "Label name cannot be a reserved word");
                        add_symbol(label, &curr, &head, 0, external);
                        break;
                    }

                    case entry:
                        break;

                    default:
                        break;
                }
                break;
            }
            case instruction:
            {
                int src, dest, i;
                src = ZERO;
                dest = ZERO;
                i=0;
                if(label_flag) 
                    add_symbol(label, &curr,&head, IC, code);
                if(op_count != get_instruction_operands(argument)) 
                    error(am_file ,"Invalid number of operands");

                if(op_count==1)
                    dest = get_mode(operands[i]);
                else if(op_count==2)
                {
                    src = get_mode(operands[i++]); 
                    dest = get_mode(operands[i]);
                }
                    
                encode_instruction(argument, src, dest);
                i = 0;
                while (i<op_count)
                    encode_operand(operands[i++]);
                break;
            }
        }
    }
    
    /* finished reading file */
    if(am_file->error_flag) return;

    ICF = IC;   
    DCF = DC;

    update_symbols(head);


        save_symbol_table(head, "test_output/first_pass.txt"); /* TODO: delete */
        save_machine_images("test_output/first_pass.txt");

    second_pass(am_file, head); /* starting second pass */
}

static void update_symbols(symbol *head)
{
    symbol *temp = head;
    while(temp != NULL)
    {
        if(temp->atr == data)
            temp->address += ICF;
        temp = temp->next;
    }
}
