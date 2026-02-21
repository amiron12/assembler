
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


int is_empty_line(char *line)
{
    if(line==NULL) return TRUE;
    while(*line != '\0')
    {
        if(!isspace(*line)) return FALSE;
        line++;
    }
    return TRUE;
}


char *add_extention(char *file_name, char *ext)
{
    char new_file[MAX_FNAME];
    strcpy(new_file, file_name);
    strcat(new_file, ext);
    return new_file;
}



