
/*
 * second_pass.c
 * This file implements the second pass of the assembler.
 * It reads the file line by line, resolves symbolic addresses, encodes 
 * entry and external symbols, and builds the final machine code image.
 * Finally, it generates the output files (.ob, .ent, .ext) or deletes them on error.
 */

#include "second_pass.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "symbols.h"
#include "output.h"
#include "string_utils.h"
#include "machine_image.h"
#include "structs.h"
#include "constants.h"
#include "utils.h"

/**
 * This function performs the second pass of the assembler on a given file.
 * It reads the file line by line, resolves symbolic addresses, encodes 
 * entry and external symbols, and builds the final machine code image.
 * Finally, it generates the output files (.ob, .ent, .ext) or deletes them on error.
 */
void second_pass(file_data *file, char* fname, char* am_extended_name)
{
    int create_ent, create_ext;
    char buffer[MAX_LINE_LENGTH]; /* Set to 81, validation is already done in first pass */
    FILE* am_file;

    am_file = fopen(am_extended_name, "r");
    if (am_file == NULL)
    {
        error(file, am_extended_name, "Error opening file");
        free_data(file);
        return;
    }
    file->IC = MEM_START;
    file->current_line = ZERO;
    create_ent = create_ext = FALSE; /* flags that indicate if a entry/external value exists */
    
    if(init_output_files(file, fname) == ERR) /* making the output files to write in, file errors are handled inside, and memory is freed in case of failure*/
    {
        fclose(am_file);
        return;
    }

    while (fgets(buffer, MAX_LINE_LENGTH, am_file) != NULL) /* line length is already handled in the first pass */
    { 
        char *operands[LINE_LENGTH], *argument, *line;
        int op_count;
        symbol *temp;
        line = buffer;
        file->current_line++;

        get_next_word(&line, &argument); /* assigning the first word to argument, line is set with the remaining text */
        
        if(argument[strlen(argument)-1] == ':') /* skiping symbols */
            get_next_word(&line, &argument);

        if (is_data(argument) || is_string(argument) || is_extern(argument))
            continue;

        op_count = tokenize(line, operands, file, am_extended_name); /* parsing through the line and inserting each word into the operands array, returns the number of words found */

        if (is_entry(argument))
        {
            if ((temp = get_symbol(*operands, file->symbol_list)))
            {
                if(is_attribute(temp, external))
                    error(file, am_extended_name, "Symbol cannot be defined as both entry and external");
                else
                {
                    set_attribute(temp, entry); /* adding the entry attribute */
                    if(!append_entry(temp, file, fname)) /* writing it to the .ent file */
                        break; /* file handling fail */
                    create_ent = TRUE; 
                }
            }
            else
                error(file, am_extended_name, "symbol not found");
        }

        else 
        {
            int i = 0;
            file->IC++;
            while (i < op_count) /* parsing through the operands */
            {
                if(file->code_image[IC_INDEX(file)].type == UNKNOWN) /* address not set */
                {    
                    if((temp = get_symbol(operands[i], file->symbol_list)))
                    {
                        if (is_attribute(temp, external))
                        {
                            file->code_image[IC_INDEX(file)].word = ZERO;
                            file->code_image[IC_INDEX(file)].type = EXTERNAL;
                            if(!append_external(temp, file, fname)) /* writing it to the .ext file */
                                break; /* file handling fail */
                            create_ext = TRUE;
                        }
                        else
                        {
                            file->code_image[IC_INDEX(file)].word = temp->address;
                            file->code_image[IC_INDEX(file)].type = RELOCATABLE;
                        }
                    }
                    else
                        error(file, am_extended_name, "symbol not found");
                }

                if(is_relative(operands[i]))
                {
                    clean_string(&operands[i], '%');
                    if((temp = get_symbol(operands[i], file->symbol_list)))
                    {
                        if(is_attribute(temp, external))
                        {
                            error(file, am_extended_name, "Relative symbol cannot be external");
                            continue;
                        }
                        file->code_image[IC_INDEX(file)].word = ((temp->address)- file->IC);
                        file->code_image[IC_INDEX(file)].type = ABSOLUTE;
                    }
                    else
                        error(file, am_extended_name, "symbol not found");
                }
                file->IC++;
                i++;
            }   
        }
    }
    fclose(am_file);

    if (!file->error_flag) /* No errors means we can create the object files */
        create_obj_file(file, fname);

    if(!create_ent)
        delete_ent_file(fname); /* no entry symbols, we dont need this output file */

    if(!create_ext)
        delete_ext_file(fname); /* no external symbols, we dont need this output file */

    if(file->error_flag)
       delete_output_files(fname);

    free_data(file); /* at the end of each process - freeing all allocated memory related to this file (macro & symbol list) */
}
