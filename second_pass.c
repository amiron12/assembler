#include "second_pass.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "symbols.h"
#include "output.h"
#include "text_parsing.h"
#include "machine_image.h"
#include "structs.h"
#include "constants.h"
#include "utils.h"

void second_pass(file_data *am_file)
{
    int create_ent, create_ext;
    char buffer[LINE_LENGTH];
    IC = MEM_START;
    am_file->current_line = ZERO;
    rewind(am_file->ptr);
    init_output_files(am_file->name); /* making the output files to write in */
    create_ent = create_ext = FALSE; /* flags that indicate if a entry/external value exists */

    while (fgets(buffer, LINE_LENGTH, am_file->ptr) != NULL)
    {
        char *operands[MAX_OPERANDS], *argument, *line;
        int op_count;
        symbol *temp;

        am_file->current_line++;
        line = buffer;

        get_next_word(&line, &argument);
        
        if(argument[strlen(argument)-1] == ':') /* skiping symbols */
            get_next_word(&line, &argument);

        if (is_data(argument) || is_string(argument) || is_extern(argument))
            continue;

        op_count = tokenize(line, operands, am_file);

        if (is_entry(argument))
        {
            if ((temp = get_symbol(*operands, am_file->symbol_list)))
            {
                if(is_attribute(temp, external))
                    error(am_file, "Symbol cannot be defined as both entry and external");
                else
                {
                    set_attribute(temp, entry); /* adding the entry attribute */
                    append_entry(temp, am_file->name); /* writing it to the .ent file */
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
                            append_external(temp, am_file->name); /* writing it to the .ext file */
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
    
    if(am_file->error_flag)
    {
        abort_file(am_file);
        return;
    }
    if(!create_ent)
        delete_ent_file(am_file->name);
    if(!create_ext)
        delete_ext_file(am_file->name);

    create_obj_file(am_file->name, am_file->symbol_list);
    

    
}
