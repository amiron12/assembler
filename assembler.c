
#include "file_utils.h"
#include "assembler.h"
#include "utils.h"
#include "constants.h"
#include "pre_proccess.h"
#include "first_pass.h"
#include "second_pass.h"
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int IC, DC, ICF, DCF;

void init_file_state(file_state *fs, char *fname, char *ext);

int main(int argc, char *argv[])
{
    int i;
    char *fname;    
    file_state am_file;
    file_state as_file;
    symbol *symbol_table;
    i = 0;
    
    while(++i<argc)
    {
        DC = ZERO;
        IC = MEM_START;
        fname = argv[i];

        init_file_state(&as_file, fname, AS);
        if(as_file.error_flag) continue;

        expand_macros(&as_file); /* pre-assembler stage */
        if(as_file.error_flag) continue; /* error occurred during macro expansion, continuing to the next file */

        init_file_state(&am_file, fname, AM);
        if(am_file.error_flag) continue;
        
        symbol_table = start_pass(&am_file); 
        if(am_file.error_flag) continue; /* error occurred during the passes, continuing to the next file */

        /* TODO: create .ob, .ext, .ent */


        fclose(am_file.ptr);
        fclose(as_file.ptr);
    }

    
    printf("finished\n"); 
    return 0;
}


