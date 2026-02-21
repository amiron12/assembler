
#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


 /* TODO: move to header file: */

 /* 31 chars */

int DC; /* data counter: .data .string */
int IC; /* instruction counter: .text */



int main(int argc, char *argv[])
{
    int i;
    for(i=1;i<argc;i++)
    {
        char *fname;    
        file_state am_file;
        file_state as_file;
        fname = argv[i];
        strncpy(as_file.name, fname, MAX_FNAME);
        extention(&as_file, EXT_AS);
        as_file.ptr = fopen(as_file.extended_name, "r");
        file_check(as_file.ptr);
        expand_macros(&as_file);
        if(!as_file.error_flag) continue;
        /* am file was created */
        strncpy(am_file.name, fname, MAX_FNAME);
        extention(&am_file, EXT_AM);
        am_file.ptr = fopen(am_file.extended_name, "r");
        file_check(am_file.ptr);
        start(&am_file);
        
        print_machine_images();
        
        printf("finished\n"); 
        exit(0);

        }
    return 0;
}




