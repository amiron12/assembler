#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "string_utils.h"
#include "output.h"
#include "symbols.h"
#include "const_tables.h"
#include "structs.h" 
#include "constants.h"

/* This function is called to check a pointer after memory allocation, and exits with a fatal error if allocation failed */
void memory_check(void *ptr)
{
    if(ptr) return;
    printf("Fatal Error: Memory alocation failed\n");
    exit(1);
}

/* This function receives a file data pointer and a string, 
    prints the error string to the standard output and turns on the file's error flag */
void error(file_data *fs, char *str)
{
    printf("[%s:%d] -> %s\n", fs->extended_name, fs->current_line, str);
    fs->error_flag = TRUE;
}

int is_directive(char *str)
{
    return (*str=='.')?TRUE:FALSE;
}

int is_data(char *str)
{
    return !strcmp(str, ".data")?TRUE:FALSE;
}

int is_entry(char *str)
{
    return !strcmp(str, ".entry")?TRUE:FALSE;
}

int is_extern(char *str)
{
    return !strcmp(str, ".extern")?TRUE:FALSE;
}

int is_string(char *str) 
{
    return !strcmp(str, ".string")?TRUE:FALSE;
}

int is_immediate(char *str)
{
    return (*str=='#')?TRUE:FALSE;
}

int is_relative(char *str)
{
    return (*str=='%')?TRUE:FALSE;
}

int is_instruction(char *str) 
{
    char *temp;
    int i = 0;
    while((temp = get_instruction_name(i++)) != NULL)
        if(!strcmp(str, temp))
            return TRUE;
    return FALSE;
}

int is_register(char *str)
{
    if(strlen(str)!=2) return FALSE;
    return(str[0]=='r' && (str[1] >= '0' && str[1] <= '7'))?TRUE:FALSE; /* TODO: change nums */
}

/* general use function to check if a given string is a reserved word of the machine */
int reserved(char *str)
{
    if(is_instruction(str) || is_register(str) || is_data(str) || is_entry(str) || is_extern(str) || is_string(str)) return TRUE;
    if(!strcmp(str, MACRO_START) || !strcmp(str, MACRO_END)) return TRUE;
    return FALSE;
}

/* general use function to make sure that a symbol is defined by the correct rules */
void validate_symbol(char *str, file_data *fs)
{
    int i, len;
    if(str == NULL)
        error(fs, "Symbol not specified");
    
    len = strlen(str);
    
    if(str[len-1]==':') /* if its a symbol defenition in the start of a line */
    {
        clean_string(&str, ':');
        len--;
    }

    if(len > MAX_SYMBOL_LENGTH)
        error(fs, "Symbol name is too long"); /* symbol too long */

    if (!isalpha(*str))
        error(fs, "Invalid symbol name"); /* first character is non alphabetical */
    
    if(is_macro_call(str, fs->macro_list))
        error(fs, "Symbol cannot be a macro name");
    
    if(symbol_exists(str, fs->symbol_list))
        error(fs, "Symbol already exists");
    
    if(reserved(str))
        error(fs, "Symbol cannot be a reserved word");

    for(i=0;i<len;i++)
        if(!isalnum(str[i]) && str[i]!='_')
            error(fs, "Invalid symbol name"); /* symbol contains a non alpha-numeric character */
    
}

/* This function returns the addressing mode of a given string */
int get_mode(char *str)
{
    if(is_immediate(str)) return IMM;
    if(is_relative(str)) return REL;
    if(is_register(str)) return REG;
    return DIR;
}

/* This function receives a string and checks if it is a valid number, with no extra characters, that fits in a 12 bit word
    returns false if there the number is not valid, and true otherwise */
int validate_number(char *str)
{
    char *ptr;
    long result;
    result = strtol(str, &ptr, DEC);
    if(ptr == str) return FALSE;
    if(*ptr != '\0') return FALSE;
    if(result > MAX_NUM || result < MIN_NUM) return FALSE;
    return TRUE;
}

/* This function receives an array of strings, goes through all and check that it is a valid number
    returns false if there is a value that is not valid or if the array is empty, and true otherwise */
int validate_data(char *integers[], file_data *fs)
{
    int i = 0;
    if(integers == NULL) return FALSE;
    while (integers[i] != NULL)
    {
        if(!validate_number(integers[i++]))
        {
            error(fs, "Invalid data value");
            return FALSE;
        }
    }
    return TRUE;
}

/* This function receives a string and the file's data, and validates the string by the assembler's rules
    returns false if non valid, and true otherwise */
int validate_string(char *str, file_data *fs)
{
    if(str == NULL)
    {
        error(fs, "String not specified");
        return FALSE;
    }

    if(str[0] == '\"' && str[strlen(str)-1] == '\"')
        return TRUE;

    error(fs, "Invalid string value");
    return FALSE;
}

/* function that wraps different memory freeing functions
all free functions are called, those that receive NULL pointers return immidietly */
void free_data(file_data *f)
{
    free_symbols(f->symbol_list);
    free_macros(f->macro_list);
    fclose(f->ptr);
}

   