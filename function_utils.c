
#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int i;
    char *temp;
    i = 0;
    /* checking instruction's table */
    while((temp = get_instruction_name(i++)) != NULL)
        if(strcmp(str, temp)==0)
            return FALSE;
    return TRUE;    
}

