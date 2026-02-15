
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
    int IC = MEM_START;
    int DC = ZERO;
    for(i=1;i<argc;i++)
        {
        char fname[100], fname2[100];
        strcpy(fname, argv[i]);
        strcpy(fname2, argv[i]);
        strcat(fname, EXT_AS);
        strcat(fname2, EXT_AM);
        fps = fopen(fname, "r");
        file_check(fps);
        fpm = fopen(fname2, "w+");
        file_check(fpm);
        expand_macros(fps, fpm);
        rewind(fpm);
        file_check(fpm);
        start(fpm);
        fclose(fps);
        fclose(fpm);
        }
    return 0;
}




