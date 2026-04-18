/*
 * first_pass.c
 * This file implements the first pass of the assembler.
 * It reads the source code line by line, identifies macro definitions and symbol definitions,
 * validates operands and directives, encodes data and strings, and partially encodes instructions.
 * Finally, it updates the symbol table addresses.
 */

#include "first_pass.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "symbols.h"
#include "machine_image.h"
#include "string_utils.h"
#include "const_tables.h"
#include "second_pass.h"
#include "constants.h"
#include "structs.h"
#include "utils.h"

/**
 * This helper function loops through all data symbols and adds the final 
 * instruction counter (ICF) to their addresses, so they follow the instruction block in memory.
 */
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

/**
 * This function performs the first pass of the assembler on a given file.
 * It reads the file line by line, handles symbol definitions, validates 
 * operands and directives, encodes data and string directives, and encodes
 * instructions without resolving forward references.
 * Finally, it updates data symbol addresses and calls the second pass.
 */
void first_pass(file_data *am_file)
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
            free_data(am_file); /* freeing the symbol and macro list */
            return; /* stop proccessing this file */
        }

        if (strlen(buffer) > MAX_LINE_LENGTH) 
        {
            error(am_file, "Line is too long");
            /* If the buffer doesn't contain a newline, the line was cut off */
            if (buffer[strlen(buffer) - 1] != '\n') 
            {
                int c;
                while ((c = fgetc(am_file->ptr)) != '\n' && c != EOF); /* consuming the rest of the line from the buffer */
            }
            continue; /* skipping this line */
        }
        
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
                add_symbol(symbol, &head, DC, data); /* If symbol is null, meaning no symbol was found, it will return without anything added */
                if (validate_data(operands, am_file))
                    encode_data(operands);
            }

            else if(is_string(argument)) /* .string */
            {
                add_symbol(symbol, &head, DC, data); /* If symbol is null, meaning no symbol was found, it will return without anything added */
                if (validate_string(*operands, am_file))
                    encode_string(*operands);      
            }

            else if(is_extern(argument)) /* .extern */
            {   
                if(op_count > ONE) 
                    error(am_file, "Extraneous text after symbol");
                symbol = *operands; /* first argument after .extern */
                validate_symbol(symbol, am_file);
                add_symbol(*operands, &head, ZERO, external);
            }

            else if(is_entry(argument)) 
            {
                if(op_count > ONE) 
                    error(am_file, "Extraneous text after symbol");
            }
                
            else
                error(am_file, "Invalid directive");
        }

        else if(is_instruction(argument))
        {
            add_symbol(symbol, &head, IC, code); /* If symbol is null, it will return without anything added */
            if (op_count == get_instruction_operands(argument))
                encode_instruction(argument, operands[0], operands[1], am_file);
            else
                error(am_file, "Invalid number of operands");
        }

        else
            error(am_file, "Invalid operation");
    }
    /* finished reading file */
    am_file->symbol_list = head;
    head = NULL;
    if(am_file->error_flag) 
    {   /* error found during the first pass, stoping the proccess and continuing to the next file */
        free_data(am_file); /* freeing the symbol and macro list */
        return;
    }

    ICF = IC;   
    DCF = DC;

    update_symbols(am_file->symbol_list);
    return;
}
