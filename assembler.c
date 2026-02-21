
#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 /* TODO: move to header file: */

 /* 31 chars */

int DC; /* data counter: .data .string */
int IC; /* instruction counter: .text */




int main(int argc, char *argv[])
{
    int i;
    FILE *fps, *fpm;
 
    for(i=1;i<argc;i++)
    {
        char *fname;    
        file_state am_file;
        file_state as_file = {NULL, NULL, NULL, 0,0};
        fname = argv[i];

        as_file.name = fname;
        as_file.extended_name = add_extention(fname, EXT_AS);
        as_file.ptr = fopen(as_file.extended_name, "r");
        file_check(as_file.ptr);
        expand_macros(&as_file);
        if(!as_file.error_flag) continue;
        /* am file was created */
        am_file.name = fname;
        am_file.extended_name = add_extention(fname, EXT_AM);
        am_file.ptr = fopen(am_file.extended_name, "r");
        file_check(am_file.ptr);

        start(&am_file);

        rewind(fpm);
        file_check(fpm);
        fclose(fps);
        fclose(fpm);
        }
    return 0;
}




