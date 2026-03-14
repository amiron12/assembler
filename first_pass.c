#include "first_pass.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "symbols.h"
#include "machine_image.h"
#include "text_parsing.h"
#include "const_tables.h"
#include "second_pass.h"
#include "constants.h"
#include "structs.h"
#include "utils.h"

static void update_symbols(symbol *head);

void start_pass(file_data *am_file)
{
    char line[LINE_LENGTH];
    char *operands[MAX_OPERANDS];
    int symbol_flag;
    symbol *head;
    head = NULL;
    am_file->symbol_list = head;

    fprintf(stderr,"[INFO] starting first pass\n");

    while(fgets(line, LINE_LENGTH, am_file->ptr) != NULL)
    {
        char *arg_string, *argument, *symbol;
        int op_count;
        symbol = NULL;
        symbol_flag = FALSE;
        am_file->current_line++;

        if(is_empty_line(line) || is_comment(line)) continue;

        if(strlen(line) > MAX_LINE_LENGTH)
            error(am_file, "Line is too long");

        argument = strtok(line, " \t");
        if (!argument) continue;

        if(valid_symbol_format(argument)) 
        {
            symbol = argument; 
            clean_string(&symbol); 
            validate_symbol(symbol, am_file);
            argument = strtok(NULL, " \t\n");
            symbol_flag = TRUE;
        }

        arg_string = strtok(NULL, "\n");
        op_count = tokenize(arg_string, operands, am_file);
        
        if(op_count == ERR)
            continue;
        
        if (is_directive(argument))
        {
            if (is_data(argument))
            {
                if(symbol_flag) add_symbol(symbol, &head, DC, data);
                if (!validate_data(operands))
                    error(am_file, "Invalid data value");
                encode_data(operands);
            }

            else if (is_string(argument))
            {
                if(symbol_flag) add_symbol(symbol, &head, DC, data);
                if (!validate_string(*operands))
                    error(am_file, "Invalid string value");
                encode_string(*operands); /*its a string directive*/
                }

            else if (is_extern(argument))
            {
                int i, len;
                char *symbol;
                symbol = operands[0];
                len = strlen(symbol);
                if (op_count != 1)
                    error(am_file, "Extraneous text after external value");
                else if (!isalpha(*symbol))
                    error(am_file, "Invalid symbol name"); /* first character is non alphabetical */
                else if ((len--) > SYMBOL_LENGTH)
                    error(am_file, "symbol name is too long");
                else if (symbol_exists(symbol, head))
                    error(am_file, "symbol name already exists");
                else if (reserved(symbol))
                    error(am_file, "symbol name cannot be a reserved word");
                else
                {
                    for (i = 1; i < len; i++)
                        if (!isalnum(symbol[i]))
                            error(am_file, "Invalid symbol name");
                }

                add_symbol(symbol, &head, 0, external);
            }

            else if (is_entry(argument))
                continue;

            else
            {
                error(am_file, "Invalid directive");
                continue;
            }
        }

        else if (is_instruction(argument))
        {
            if (symbol_flag) 
                add_symbol(symbol, &head, IC, code);
            if (op_count == get_instruction_operands(argument))
                encode_instruction(argument, operands[0], operands[1], am_file);
            else
            {
                error(am_file, "Invalid number of operands");
                continue;
            }
        }

        else
            error(am_file, "Invalid operation");
    }

    fprintf(stderr,"[INFO] Done reading file\n");
    
    /* finished reading file */
    if(am_file->error_flag) 
    {
        abort_file(am_file);
        return;
    }

    ICF = IC;   
    DCF = DC;

    update_symbols(head);


        save_symbol_table(head, "test_output/first_pass.txt"); /* TODO: delete */
        save_machine_images("test_output/first_pass.txt");
    
    fprintf(stderr,"[INFO] finished first pass\n");

    second_pass(am_file, head); /* starting second pass */
}


static void update_symbols(symbol *head)
{
    symbol *temp = head;
    while(temp != NULL)
    {
        if(is_attribute(temp, data))
            temp->address += ICF;
        temp = temp->next;
    }
}
