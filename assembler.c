
#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    int i;
    DC = ZERO;
    IC = MEM_START;
    for(i=1;i<argc;i++)
    {
        char *fname;    
        file_state am_file;
        file_state as_file;
        fname = argv[i];
        strncpy(as_file.name, fname, MAX_FNAME);
        extention(&as_file, EXT_AS);
        as_file.ptr = fopen(as_file.extended_name, "r");
        as_file.error_flag = FALSE;
        as_file.current_line = ZERO;
        file_check(as_file.ptr);

        expand_macros(&as_file); /* pre-assembler stage */
        if(as_file.error_flag) continue;

        strncpy(am_file.name, fname, MAX_FNAME);
        extention(&am_file, EXT_AM);
        am_file.ptr = fopen(am_file.extended_name, "r");
        file_check(am_file.ptr);
        
        start(&am_file); /* starting first pass */
        if(am_file.error_flag) exit(0); /* error */




        print_machine_images();
        printf("finished\n"); 
        }
    return 0;
}



