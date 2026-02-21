
#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>




static char* trim_space(char *str);
static int validate_commas(char *input);


/*
 * Function: tokenize
 * Purpose: Splits a line into arguments based on commas.
 * Logic: Validates syntax first, then uses strtok to split by comma and trims spaces.
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
        return NEG;
    }

    if(validate_commas(line)!=OK) /* checking general input correction */
        return NEG;
    
    token = strtok(line, ","); 
    while(token != NULL)
        {
            args[i++] = trim_space(token); 
            token = strtok(NULL, ",");
        }
    args[i]=NULL; /* end of arguments */
    return i;
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



/*
 * Function: validate_commas
 * Purpose: Validates the syntax of the input line (commas, spaces).
 * Logic: Ensures commas are not consecutive, not at start/end, and properly separate tokens.
 * Assumptions: input is null-terminated.
 */
static int validate_commas(char *input)
 {
    char COMMA = ',';
    int expecting_comma = ZERO; /* Flag: 1 if comma expected, 0 otherwise */
    char *last_char; /* Pointer to the last character of the string */

 
    if (input == NULL || *input == '\0') return !OK;
    last_char = input + strlen(input) - ONE;

    while (last_char > input && isspace((unsigned char)*last_char))
        last_char--;
    
    if (*last_char == COMMA)
    {
        printf("< Extraneous text after end of command >\n");
        return !OK;
    }

    while (*input != '\0')
    {
        if (isspace(*input)) {
            input++;
            continue;
        }

        if (*input == COMMA) {
            if (expecting_comma == ZERO) {
                printf("< Unexpected comma or multiple commas >\n");
                return !OK;
            }
            expecting_comma = ZERO; 
        } else {
            if (expecting_comma == ONE) {
                printf("< Missing comma between parameters >\n");
                return !OK;
            }

            while (*input != '\0' && !isspace(*input) && *input != COMMA) {
                input++;
            }
            expecting_comma = ONE; 
            continue; 
        }
        input++;
    }

    return OK;
}
