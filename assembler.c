/*
 * assembler.c
 * This program is the main entry point for the assembler. It coordinates the 
 * assembly process for each provided file, handling initialization, macro 
 * expansion (pre-processing), and the two main assembly passes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "machine_image.h"
#include "pre_proccess.h"
#include "first_pass.h"
#include "second_pass.h"
#include "output.h"
#include "constants.h"
#include "structs.h"
#include "utils.h"

/**
 * This helper function resets the file's state variables, such as 
 * instruction and data counters, and clears the memory images.
 * It is called before processing each new file.
 */
static void reset_state(file_data* file)
{
    file->current_line = ZERO;
    file->DC = ZERO;
    file->IC = MEM_START;
    file->ICF = file->DCF = ZERO;
    file->macro_list = NULL;
    file->symbol_list = NULL;
    memset(file->code_image, ZERO, sizeof(file->code_image));
    memset(file->data_image, ZERO, sizeof(file->data_image));
    file->error_flag = FALSE;
}

/**
 * The main entry point of the assembler program. It loops through all 
 * provided assembly files, initializes their state, expands macros, 
 * and orchestrates the first and second pass.
 */
int main(int argc, char *argv[])
{
    char *fname;
    file_data file;
    int i = 0;
    char as_extended_name[MAX_FILE_NAME];
    char am_extended_name[MAX_FILE_NAME]; /* Name with .as, .am, etc. attached */
    while(++i<argc)
    {
        reset_state(&file); /* resetting the file state's variables */
        fname = argv[i];
        /* initializing the file names with the extended names */
        if (!init_file_data(as_extended_name, fname, AS) || !init_file_data(am_extended_name, fname, AM))
        {
            error(&file, fname, "File name is too long");
            continue;
        }

        expand_macros(&file, as_extended_name, am_extended_name); /* pre-assembler stage - removing comments, empty lines, and expanding macros */
        
        if(file.error_flag) continue; /* error occurred during macro expansion, continuing to the next file */
        first_pass(&file, am_extended_name); /* first pass - validating symbols, instructions, and operands */

        if(file.error_flag) continue; /* error occurred during first pass, continuing to the next file */
        second_pass(&file, fname, am_extended_name); /* second pass - generating the machine image */
    }
    return 0;
}
