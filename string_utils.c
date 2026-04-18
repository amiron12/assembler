/*
 * string_utils.c
 * This file contains utility functions for string manipulation and parsing. 
 * It includes functions for trimming whitespace, validating argument formats 
 * (like comma placement), tokenizing lines into operands, identifying comments 
 * and empty lines, and modifying strings for processing.
 */

#include "string_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "structs.h"
#include "constants.h"
#include "utils.h"

/**
 * This helper function trims leading and trailing whitespace characters 
 * from a given string. It modifies the string in place and returns a pointer
 * to the first non-whitespace character.
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

/**
 * This helper function validates the placement of commas in a string of arguments.
 * It checks for illegal commas before the first operand, consecutive commas,
 * missing commas between operands, and extraneous commas at the end of the command.
 * Returns TRUE if the comma placement is valid, and FALSE otherwise, while 
 * printing the appropriate error message using the file_data structure.
 */
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

/**
 * This function receives a pointer to a line of text, extracts the first word 
 * (which ends with the first whitespace character) and updates the original 
 * pointer to point to the remainder of the line.
 * It is used for parsing the initial command or label from a line of assembly code.
 */
void get_next_word(char **line, char **word)
{
    *word = strtok(*line, " \t\n\r");
    *line = strtok(NULL, "");
}

/**
 * This function takes a line of text containing comma-separated arguments,
 * validates the comma placement, and splits the line into an array of strings (tokens).
 * Each token is trimmed of leading and trailing whitespaces.
 * It returns the number of tokens found, or ERR if the comma validation fails.
 */
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

/**
 * This function checks whether a given string consists entirely of whitespace
 * characters or is completely empty. Returns TRUE if it is empty or only whitespace,
 * otherwise returns FALSE.
 */
int is_empty_line(char *buff) {
    if (buff == NULL) return TRUE;
    while (*buff) 
    {
        if(!isspace((unsigned char)*buff)) return FALSE;
        buff++;
    }
    return TRUE;
}

/**
 * This function determines if a given line is a comment. A comment line is 
 * defined as a line where the first non-whitespace character is a semicolon (';').
 * Returns TRUE if the line is a comment, FALSE otherwise.
 */
int is_comment(char *line)
{
    if(line==NULL) return FALSE;
    while(isspace(*line))
       line++;
    if(*line == ';') return TRUE;
    return FALSE;
}

/**
 * This function receives a file_data pointer and a file extension string.
 * It combines the base file name from the file_data structure with the given
 * extension and stores the result in the extended_name field of the structure.
 */
void extention(file_data *fs, char *ext)
{
    char fname[MAX_FILE_NAME];
    strcpy(fname, (fs)->name);
    strcat(fname, ext);
    strncpy((fs)->extended_name, fname, MAX_FILE_NAME);
}

/**
 * This function removes specific non-alphanumeric symbols from a string based on 
 * the provided character parameter and the assemblers rules. The string pointer is 
 * updated in place.
 */
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
