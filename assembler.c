
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

int IC, DC, ICF, DCF;

/**
 * This helper function resets the global assembler state variables, 
 * such as instruction and data counters, and clears the memory images.
 * It is called before processing each new file.
 */
static void reset_state()
{
    DC = ZERO;
    IC = MEM_START;
    ICF = DCF = ZERO; 
    memset(code_image, ZERO, sizeof(code_image));
    memset(data_image, ZERO, sizeof(data_image));
}

/**
 * The main entry point of the assembler program. It loops through all 
 * provided assembly files, initializes their state, expands macros, 
 * and orchestrates the first and second pass.
 */
int main(int argc, char *argv[])
{
    char *fname;    
    file_data am_file;
    file_data as_file;
    int i = 0;
    while(++i<argc)
    {
        reset_state();
        fname = argv[i];
        init_file_data(&as_file, fname, AS, "r");
        init_file_data(&am_file, fname, AM, "w+");

        if(as_file.error_flag || am_file.error_flag) continue; /* error initializing */
        expand_macros(&as_file, &am_file); /* pre-assembler stage */
        
        if(as_file.error_flag || am_file.error_flag) continue; /* error occurred during macro expansion, continuing to the next file */
        first_pass(&am_file); 

        if(am_file.error_flag) continue; /* error occurred during first pass, continuing to the next file */
        second_pass(&am_file);
    }
    return 0;
}
