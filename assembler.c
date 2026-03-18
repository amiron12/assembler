#include "assembler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "pre_proccess.h"
#include "first_pass.h"
#include "second_pass.h"
#include "output.h"
#include "constants.h"
#include "structs.h"
#include "utils.h"

int IC, DC, ICF, DCF;

void reset_state();

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

        if(as_file.error_flag || am_file.error_flag) continue; /* error initializeing */

        expand_macros(&as_file, &am_file); /* pre-assembler stage */
        
        if(as_file.error_flag || am_file.error_flag) continue; /* error occurred during macro expansion, continuing to the next file */
        
        start_pass(&am_file); 
    }
    return 0;
}


void reset_state()
{
    DC = ZERO;
    IC = MEM_START;
    ICF = DCF = ZERO; 
    memset(code_image, ZERO, sizeof(code_image));
    memset(data_image, ZERO, sizeof(data_image));
}
