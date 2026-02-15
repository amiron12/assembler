
#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



static char* trim_space(char *str);

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
        return OK;
    }

    
    token = strtok(line, ","); 
    while(token != NULL)
        {
            args[i++] = trim_space(token); 
            token = strtok(NULL, ",");
        }
    args[i]=NULL; /* end of arguments */
    return OK;
}

/*
 * Function: trim_space
 * Purpose: Removes leading and trailing whitespace from a string.
 * Logic: Advances pointer past leading spaces, places null terminator after last non-space.
 * Assumptions: str is a valid string pointer.
 */
static char* trim_space(char *str)
{
    char *end; /* Pointer to the end of the string */
    if(str == NULL) return NULL;

    while (*str != '\0' && isspace((unsigned char)*str))
        str++;

    if (*str == '\0')
        return str;

    end = str + strlen(str) - ONE;
    while (end > str && isspace((unsigned char)*end))
        end--;
    end[ONE] = '\0';
    return str;
}