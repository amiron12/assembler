
#include "assembler.h"
#include "output.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



/* pointer validations */

void memory_check(void *ptr)
{
    if(ptr) return;
    printf("Fatal Error: Memory alocation failed\n");
    exit(1);
}

void file_check(FILE *fp) /* TODO: delete */
{
    if(fp) return;
    printf("Fatal Error: File opening failed\n");  
    exit(1);
}


void error(file_state *fs, char *str)
{
    printf("File: %s Line: %d - %s\n", fs->extended_name, fs->current_line, str);
    fprintf(stderr,"[ERROR] File: %s Line: %d - %s\n", fs->extended_name, fs->current_line, str);
    fs->error_flag = TRUE;
}



void extention(file_state *fs, char *ext)
{
    char fname[MAX_FNAME];
    strcpy(fname, (fs)->name);
    strcat(fname, ext);
    strncpy((fs)->extended_name, fname, MAX_FNAME);
}



int not_reserved(char *str)
{
    if(is_instruction(str) || is_register(str)) return FALSE;
    return TRUE;
}
