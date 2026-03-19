#include "string_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "structs.h"
#include "constants.h"
#include "utils.h"

static char* trim_space(char *str);
static int validate_commas(char *input, file_data *fs);


/* this function receives a pointer to a line of text, extracts the first word (ends with the first whitespace)
 and leaves the original pointer as the remaining of the line */
void get_next_word(char **line, char **word)
{
    *word = strtok(*line, " \t\n\r");
    *line = strtok(NULL, "");
}

int tokenize(char *line, char *args[], file_data *fs)
{
    char *token; /* Pointer to the current token */
    int i = ZERO;
    token = NULL;
    memset(args, 0, MAX_OPERANDS*sizeof(char *)); /* reseting the array */

    /* checking if the input is empty */
    if(line == NULL || is_empty_line(line))
        return ZERO; 

    
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

/* This function receives a file data pointer, and an extention
 it sets the pointer's extended name with the extension */
void extention(file_data *fs, char *ext)
{
    char fname[MAX_FILE_NAME];
    strcpy(fname, (fs)->name);
    strcat(fname, ext);
    strncpy((fs)->extended_name, fname, MAX_FILE_NAME);
}

/* this functions removes the non alpha-numeric symbols from a string, depending on the symbol given as a parameter
it is built like this so error correction can be successful on the string returning */
void clean_string(char **str, char c)
{
    switch (c)
    {
    case (':'):
        (*str)[strlen(*str)-1] = '\0';
        break;
    case ('#'):
        (*str)++;
        break;
    case ('%'):
        (*str)++;
        break;
    case ('"'):
        (*str)++;
        (*str)[strlen(*str)-1] = '\0';
        break;
    }
}