
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
    if(is_instruction(str) || is_register(str)) return FALSE;
    return TRUE;
}
