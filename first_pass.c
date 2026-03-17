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
    char buffer[LINE_LENGTH];
    char *operands[MAX_OPERANDS];
    int symbol_flag;
    symbol *head;
    head = NULL;

    fprintf(stderr,"[INFO] starting first pass\n");

    while(fgets(buffer, LINE_LENGTH, am_file->ptr) != NULL)
    {
        char *argument, *symbol, *line;
        int op_count;
        symbol = NULL;
        symbol_flag = FALSE;
        am_file->current_line++;
        line = buffer;

        if(strlen(buffer) > MAX_LINE_LENGTH)
            error(am_file, "Line is too long");
        
        get_next_word(&line, &argument); /* assigning the first word to argument, line is set with the remaining text*/

        if(argument[strlen(argument)-1] == ':')
        {
            symbol = argument; 
            validate_symbol(symbol, am_file); /* if there is an error the flag will be raised, continuing to read file anyway */
            get_next_word(&line, &argument);
            symbol_flag = TRUE;
        }
    
        op_count = tokenize(line, operands, am_file); /* parsing through the line and inserting each word into the operands array, returns the number of words found */
        
        if(op_count == ERR) /* skipping a line with syntax errors, operands cannot be parsed */
            continue;
        
        if (is_directive(argument))
        {
            if (is_data(argument)) /* .data */
            {
                if(symbol_flag) 
                    add_symbol(symbol, &head, DC, data);
                if (validate_data(operands))
                    encode_data(operands);
                else
                    error(am_file, "Invalid data value");
                continue;
            }

            if(is_string(argument)) /* .string */
            {
                if(symbol_flag) 
                    add_symbol(symbol, &head, DC, data);
                if (validate_string(*operands))
                    encode_string(*operands); /*its a string directive*/
                else
                    error(am_file, "Invalid string value");
                continue;
            }

            if(is_extern(argument)) /* .extern */
            {   
                symbol = *operands;
                if(op_count > 1)
                    error(am_file, "Extraneous text after extern directive");
                validate_symbol(symbol, am_file);
                add_symbol(symbol, &head, 0, external);
                continue;
            }

            if(is_entry(argument)) /* .entry */
            {
                if (!op_count) /* op_count is 0 */
                    error(am_file, "No symbol after entry directive");
                else if(op_count > 1)
                    error(am_file, "Extraneous text after entry directive");
                continue;
            }
                
            error(am_file, "Invalid directive");
            continue;

        }

        if(is_instruction(argument))
        {
            if (symbol_flag) 
                add_symbol(symbol, &head, IC, code);
            if (op_count == get_instruction_operands(argument))
                encode_instruction(argument, operands[0], operands[1], am_file);
            else
                error(am_file, "Invalid number of operands");
            continue;
        }

        error(am_file, "Invalid operation");
    }
    
    am_file->symbol_list = head;

    /* finished reading file */
    if(am_file->error_flag) 
    {
        abort_file(am_file);
        return;
    }

    ICF = IC;   
    DCF = DC;

    update_symbols(head);
    
    fprintf(stderr,"[INFO] finished first pass\n");

    second_pass(am_file);
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
