
#include "output.h"
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

int main(int argc, char *argv[])
{
    int i;
    char *fname;    
    file_state am_file;
    file_state as_file;
    i = 0;
    freopen("logs.txt", "w+", stderr);
    freopen("logs.txt", "a", stderr); /* TODO: delete this and every fprintf(stderr..) */
    fprintf(stderr,"\t------\n");
    while(++i<argc)
    {
        DC = ZERO;
        IC = MEM_START;
        fname = argv[i];
        
        fprintf(stderr,"[%s]\n", fname);

        init_file_state(&as_file, fname, AS, "r");

        init_file_state(&am_file, fname, AM, "w+");

        if(as_file.error_flag) continue;

        expand_macros(&as_file, &am_file); /* pre-assembler stage */
        if(as_file.error_flag) continue; /* error occurred during macro expansion, continuing to the next file */

        start_pass(&am_file); 
        if(am_file.error_flag) continue; /* error occurred during the passes, continuing to the next file */


        fclose(am_file.ptr);
        fclose(as_file.ptr);
        
    }
    
    fprintf(stderr,"\t---Finished---\n\n"); 
    return 0;
}


