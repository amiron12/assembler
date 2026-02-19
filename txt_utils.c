
#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>




static char* clean_arg(char *str);



/*
 * Function: tokenize
 * Purpose: 
 * Logic:
 * Assumptions: line is a modifiable string.
 */
int tokenize(char *line, char *args[])
{
    char *token; /* Pointer to the current token */
    int i = ZERO;
    token = NULL;
    
    /* checking if the input is empty */
    if(line == NULL)
    {
        args[i] = NULL;
        return ZERO;
    }

    
    token = strtok(line, " "); 
    while(token != NULL)
        {
            args[i++] = clean_arg(token); 
            token = strtok(NULL, " ");
        }
    args[i]=NULL; /* end of arguments */
    return i;
}


static char* clean_arg(char *str)
{
    char *end; /* Pointer to the end of the string */
    if(str == NULL) return NULL;

    while (*str != '\0' && isspace((unsigned char)*str))
        str++;

    if (*str == '\0')
        return str;

    end = str + strlen(str) - ONE;
    while (end > str && (isspace((unsigned char)*end) || *end == ','))
        end--;
    end[ONE] = '\0';
    return str;
}

