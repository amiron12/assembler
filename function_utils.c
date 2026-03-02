
#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



void memory_check(void *ptr)
{
    if(ptr == NULL)
      {
        printf("Memory alocation failed");
        exit(1);
      }
}

void file_check(FILE *fp)
{
    if(fp==NULL)
    {
        printf("File opening failed");
        exit(1);
    }
    return;
}

int not_reserved(char *str)
{
    if(is_instruction(str) || is_register(str)) return FALSE;
    return TRUE;
}


void error(file_state *state, char *str)
{
    printf("File: %s Line: %d - %s\n", state->name, state->current_line, str);
}


int is_empty_line(char *buff)
{
    int i;
    char line[MAX_LINE_LENGTH];
    if(buff==NULL) return TRUE;
    strcpy(line, buff);
    i=0;
    while(line[i] != '\0')
        if(!isspace(line[i++])) return FALSE;
    return TRUE;
}

int is_comment(char *line)
{
    if(line==NULL) return FALSE;
    while(isspace(*line))
       line++;
    if(*line == ';') return TRUE;
    return FALSE;
}

void extention(file_state *fs, char *ext)
{
    char fname[MAX_FNAME];
    strcpy(fname, (fs)->name);
    strcat(fname, ext);
    strncpy((fs)->extended_name, fname, MAX_FNAME);
}
