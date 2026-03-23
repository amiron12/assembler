
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
void second_pass(file_data *am_file)
{
    int create_ent, create_ext;
    char buffer[MAX_LINE_LENGTH]; /* Set to 81, validation is already done in first pass */
    IC = MEM_START;
    am_file->current_line = ZERO;
    rewind(am_file->ptr);
    create_ent = create_ext = FALSE; /* flags that indicate if a entry/external value exists */
    
    if(init_output_files(am_file) == ERR) /* making the output files to write in, file errors are handled inside, and memory is freed in case of failure*/
        return;

    while (fgets(buffer, MAX_LINE_LENGTH, am_file->ptr) != NULL)
    {
        char *operands[MAX_OPERANDS], *argument, *line;
        int op_count;
        symbol *temp;
        line = buffer;
        am_file->current_line++;

        get_next_word(&line, &argument); /* assigning the first word to argument, line is set with the remaining text */
        
        if(argument[strlen(argument)-1] == ':') /* skiping symbols */
            get_next_word(&line, &argument);

        if (is_data(argument) || is_string(argument) || is_extern(argument))
            continue;

        op_count = tokenize(line, operands, am_file); /* parsing through the line and inserting each word into the operands array, returns the number of words found */

        if (is_entry(argument))
        {
            if ((temp = get_symbol(*operands, am_file->symbol_list)))
            {
                if(is_attribute(temp, external))
                    error(am_file, "Symbol cannot be defined as both entry and external");
                else
                {
                    set_attribute(temp, entry); /* adding the entry attribute */
                    if(!append_entry(temp, am_file)) /* writing it to the .ent file */
                        break; /* file handling fail */
                    create_ent = TRUE; 
                }
            }
            else
                error(am_file, "symbol not found");
        }

        else 
        {
            int i = 0;
            IC++;
            while (i < op_count) /* parsing through the operands */
            {
                if(code_image[IC_INDEX].type == UNKNOWN) /* address not set */
                {    
                    if((temp = get_symbol(operands[i], am_file->symbol_list)))
                    {
                        if (is_attribute(temp, external))
                        {
                            code_image[IC_INDEX].word = ZERO;
                            code_image[IC_INDEX].type = EXTERNAL;
                            if(!append_external(temp, am_file)) /* writing it to the .ext file */
                                break; /* file handling fail */
                            create_ext = TRUE;
                        }
                        else
                        {
                            code_image[IC_INDEX].word = temp->address; 
                            code_image[IC_INDEX].type = RELOCATABLE;
                        }
                    }
                    else
                        error(am_file, "symbol not found");
                }

                if(is_relative(operands[i]))
                {
                    clean_string(&operands[i], '%');
                    if((temp = get_symbol(operands[i], am_file->symbol_list)))
                    {
                        if(is_attribute(temp, external))
                        {
                            error(am_file, "Relative symbol cannot be external");
                            continue;
                        }
                        code_image[IC_INDEX].word = ((temp->address)-IC);
                        code_image[IC_INDEX].type = ABSOLUTE;

                    }
                    else
                        error(am_file, "symbol not found");
                }
                IC++;
                i++;
            }   
        }

    }
    
    create_obj_file(am_file);

    if(!create_ent)
    delete_ent_file(am_file->name); /* no entry symbols */

    if(!create_ext)
    delete_ext_file(am_file->name); /* no external symbols */

    if(am_file->error_flag)
       delete_output_files(am_file->name);

    free_data(am_file);
}
