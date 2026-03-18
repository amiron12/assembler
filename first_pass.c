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
    char *operands[LINE_LENGTH];
    symbol *head = NULL;

    while(fgets(buffer, LINE_LENGTH, am_file->ptr) != NULL)
    {
        char *argument, *symbol, *line;
        int op_count;
        symbol = NULL;
        am_file->current_line++;
        line = buffer;
        am_file->symbol_list = head;
        
        if(IC+DC >= MEMORY) 
        {
            error(am_file, "Memory overflow");
            abort_file(am_file);
            return;
        }

        if(strlen(buffer) > MAX_LINE_LENGTH) error(am_file, "Line is too long");
        
        get_next_word(&line, &argument); /* assigning the first word to argument, line is set with the remaining text*/

        if(argument[strlen(argument)-1] == ':')
        {
            symbol = argument; 
            validate_symbol(symbol, am_file); /* if there is an error the flag will be raised, continuing to read file anyway */
            if(is_empty_line(line))
            {
                error(am_file, "Empty line after symbol definition");
                continue;
            }
            get_next_word(&line, &argument);
        }
    
        op_count = tokenize(line, operands, am_file); /* parsing through the line and inserting each word into the operands array, returns the number of words found */
        
        if(op_count == ERR) continue; /* skipping a line with syntax errors, operands cannot be parsed */
            
        if (is_directive(argument))
        {
            if (is_data(argument)) /* .data */
            {
                add_symbol(symbol, &head, DC, data); /* If symbol is null, it will return without anything added */
                if (validate_data(operands, am_file))
                    encode_data(operands);
            }

            else if(is_string(argument)) /* .string */
            {
                add_symbol(symbol, &head, DC, data); /* If symbol is null, it will return without anything added */
                if (validate_string(*operands, am_file))
                    encode_string(*operands);      
            }

            else if(is_extern(argument)) /* .extern */
            {   
                validate_symbol(*operands, am_file);
                add_symbol(*operands, &head, ZERO, external);
            }

            else if(is_entry(argument)) {/* Nothing happens in the first pass */}
                
            else
                error(am_file, "Invalid directive");
            continue;
        }

        if(is_instruction(argument))
        {
            add_symbol(symbol, &head, IC, code); /* If symbol is null, it will return without anything added */
            if (op_count == get_instruction_operands(argument))
                encode_instruction(argument, operands[0], operands[1], am_file);
            else
                error(am_file, "Invalid number of operands");
            continue;
        }

        error(am_file, "Invalid operation");
    }
    /* finished reading file */
    
    am_file->symbol_list = head;
    head = NULL;
    if(am_file->error_flag) 
    {
        free_data(am_file);
        return;
    }

    ICF = IC;   
    DCF = DC; /* TODO: need these? */

    update_symbols(head);
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
