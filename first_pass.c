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
static void update_symbols(symbol *head, int ICF)
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
 * Finally, it updates data symbol addresses.
 */
void first_pass(file_data *file, char* am_extended_name)
{
    char buffer[LINE_LENGTH];
    char *operands[LINE_LENGTH];
    FILE* am_file;
    symbol *head = NULL;
    am_file = fopen(am_extended_name, "r");
    if (am_file == NULL)
    {
        error(file, am_extended_name, "Error opening file");
        free_data(file);
        return;
    }
    file->current_line = ZERO; /* resetting the file's current line counter */
    while(fgets(buffer, LINE_LENGTH, am_file) != NULL)
    {
        char *argument, *symbol, *line;
        int op_count;
        symbol = NULL;
        file->current_line++;
        line = buffer;
        file->symbol_list = head;
        
        if(file->IC+ file->DC >= MEMORY)
        {
            error(file, am_extended_name, "Memory overflow");
            free_data(file); /* freeing the symbol and macro list */
            fclose(am_file);
            return; /* stop proccessing this file */
        }

        if (strlen(buffer) > MAX_LINE_LENGTH) 
        {
            error(file, am_extended_name, "Line is too long");
            /* If the buffer doesn't contain a newline, the line was cut off */
            if (buffer[strlen(buffer) - 1] != '\n') 
            {
                int c;
                while ((c = fgetc(am_file)) != '\n' && c != EOF); /* consuming the rest of the line from the buffer */
            }
            continue; /* skipping this line */
        }
        
        get_next_word(&line, &argument); /* assigning the first word to argument, line is set with the remaining text*/
        if(argument == NULL)
            continue;

        if(argument[strlen(argument)-1] == ':')
        {
            symbol = argument; 
            validate_symbol(symbol, file, am_extended_name); /* if there is an error the flag will be raised, continuing to read file anyway */
            if(is_empty_line(line))
            {
                error(file, am_extended_name, "Empty line after symbol definition");
                continue;
            }
            get_next_word(&line, &argument);
            if(argument == NULL)
            {
                error(file, am_extended_name, "Missing operation after symbol definition");
                continue;
            }
        }
    
        op_count = tokenize(line, operands, file, am_extended_name); /* parsing through the line and inserting each word into the operands array, returns the number of words found */
        
        if(op_count == ERR) continue; /* skipping a line with syntax errors, operands cannot be parsed */
            
        if (is_directive(argument))
        {
            if (is_data(argument)) /* .data */
            {
                add_symbol(symbol, &head, file->DC, data); /* If symbol is null, meaning no symbol was found, it will return without anything added */
                if (validate_data(operands, file, am_extended_name))
                    encode_data(operands, file);
            }

            else if(is_string(argument)) /* .string */
            {
                add_symbol(symbol, &head, file->DC, data); /* If symbol is null, meaning no symbol was found, it will return without anything added */
                if (validate_string(*operands, file, am_extended_name))
                    encode_string(*operands, file);      
            }

            else if(is_extern(argument)) /* .extern */
            {   
                if(op_count > ONE) 
                    error(file, am_extended_name, "Extraneous text after symbol");
                symbol = *operands; /* first argument after .extern */
                validate_symbol(symbol, file, am_extended_name);
                add_symbol(*operands, &head, ZERO, external);
            }

            else if(is_entry(argument)) 
            {
                if(op_count == ZERO)
                    error(file, am_extended_name, "Symbol not specified");
                else if(op_count > ONE) 
                    error(file, am_extended_name, "Extraneous text after symbol");
            }
                
            else
                error(file, am_extended_name, "Invalid directive");
        }

        else if(is_instruction(argument))
        {
            add_symbol(symbol, &head, file->IC, code); /* If symbol is null, it will return without anything added */
            if (op_count == get_instruction_operands(argument))
                encode_instruction(argument, operands[0], operands[1], file, am_extended_name);
            else
                error(file, am_extended_name, "Invalid number of operands");
        }

        else
            error(file, am_extended_name, "Invalid operation");
    }
    fclose(am_file);
    /* finished reading file */
    file->symbol_list = head;
    head = NULL;
    if(file->error_flag) 
    {   /* error found during the first pass, stoping the proccess and continuing to the next file */
        free_data(file); /* freeing the symbol and macro list */
        return;
    }

    file->ICF = file->IC;
    file->DCF = file->DC;

    update_symbols(file->symbol_list, file->ICF);
}
