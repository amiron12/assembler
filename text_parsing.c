#include "text_parsing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "structs.h"
#include "constants.h"
#include "utils.h"

/* TODO: fix comments */

static char* trim_space(char *str);
static int validate_commas(char *input, file_data *fs);


int tokenize(char *line, char *args[], file_data *fs)
{
    char *token; /* Pointer to the current token */
    int i = ZERO;
    token = NULL;
    
    /* checking if the input is empty */
    if(line == NULL)
    {
        args[i] = NULL;
        return ZERO; /* 0 operands */
    }

    
    if(!validate_commas(line, fs)) 
    {
        args[i] = NULL;
        return ERR;
    }  
    
    token = strtok(line, ","); 
    while(token != NULL)
        {
            args[i++] = trim_space(token); 
            token = strtok(NULL, ",");
        }
    args[i]=NULL; /* end of arguments */
    return i;
}


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


static int validate_commas(char *input, file_data *fs) 
{
    int expecting_operand = 1; /* expecting an operand */
    char *ptr = input;

    while (*ptr && isspace((unsigned char)*ptr))
        ptr++; /* getting to the first word */

    if (*ptr == ',') 
    {
        error(fs, "Illegal comma before operand");
        return FALSE;
    }

    while (*ptr)
    {
        if (*ptr == ',') 
        {
            if (expecting_operand) 
            {
                error(fs, "Multiple consecutive commas or misplaced comma");
                return FALSE;
            }
            expecting_operand = 1;
        } 
        else if (!isspace((unsigned char)*ptr)) 
        {
            /* found a non-space character  */
            if (!expecting_operand) 
            {
                error(fs, "Missing comma between operands");
                return FALSE;
            }
            /* skip rest of this operand */
            while (*ptr && !isspace((unsigned char)*ptr) && *ptr != ',') 
                ptr++;
            expecting_operand = 0;
            continue;
        }
        ptr++;
    }

    if (expecting_operand && ptr != input) 
        error(fs, "Extraneous comma at end of command");

    return TRUE;
}


int is_empty_line(char *buff) {
    if (buff == NULL) return TRUE;
    while (*buff) 
    {
        if(!isspace((unsigned char)*buff)) return FALSE;
        buff++;
    }
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
