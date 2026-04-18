/*
 * utils.c
 * This file contains general utility functions used throughout the assembler. 
 * It provides functions for file initialization, error handling, memory 
 * management (allocation checks, freeing data structures), validation of 
 * symbols, numbers, and strings, as well as functions to identify directives, 
 * instructions, registers, and operand addressing modes.
 */

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


/**
 * This function initializes a file_data structure with the given file name, 
 * extension, and mode. It opens the file and resets all relevant flags 
 * and pointers.
 */
void init_file_data(file_data *fs, char *fname, char *ext, char *mode)
{
    strncpy(fs->name, fname, MAX_FILE_NAME);
    extention(fs, ext);
    fs->ptr = fopen(fs->extended_name, mode);
    fs->error_flag = FALSE;
    fs->current_line = ZERO;
    fs->macro_list = NULL;
    fs->symbol_list = NULL;
    if((fs->ptr)) return;
    error(fs, "Error opening file");
}

/**
 * This function checks a pointer after memory allocation, 
 * and exits the program with a fatal error if the allocation failed.
 */
void memory_check(void *ptr)
{
    if(ptr) return;
    printf("Fatal Error: Memory alocation failed\n");
    exit(1);
}

/**
 * This function receives a file_data pointer and an error message string.
 * It prints the error message to the standard output along with the file name
 * and line number, and turns on the file's error flag.
 */
void error(file_data *fs, char *str)
{
    printf("[%s:%d] -> %s\n", fs->extended_name, fs->current_line, str);
    fs->error_flag = TRUE;
}

/**
 * This function checks if a given string starts with a dot ('.'),
 * indicating it might be a directive. Returns TRUE if it is, FALSE otherwise.
 */
int is_directive(char *str)
{
    return (*str=='.')?TRUE:FALSE;
}

/**
 * This function checks if a given string is the ".data" directive.
 * Returns TRUE if it matches, FALSE otherwise.
 */
int is_data(char *str)
{
    return !strcmp(str, ".data")?TRUE:FALSE;
}

/**
 * This function checks if a given string is the ".entry" directive.
 * Returns TRUE if it matches, FALSE otherwise.
 */
int is_entry(char *str)
{
    return !strcmp(str, ".entry")?TRUE:FALSE;
}

/**
 * This function checks if a given string is the ".extern" directive.
 * Returns TRUE if it matches, FALSE otherwise.
 */
int is_extern(char *str)
{
    return !strcmp(str, ".extern")?TRUE:FALSE;
}

/**
 * This function checks if a given string is the ".string" directive.
 * Returns TRUE if it matches, FALSE otherwise.
 */
int is_string(char *str) 
{
    return !strcmp(str, ".string")?TRUE:FALSE;
}

/**
 * This function checks if an operand string represents an immediate value
 * (starts with '#'). Returns TRUE if it does, FALSE otherwise.
 */
int is_immediate(char *str)
{
    return (*str=='#')?TRUE:FALSE;
}

/**
 * This function checks if an operand string represents a relative addressing
 * (starts with '%'). Returns TRUE if it does, FALSE otherwise.
 */
int is_relative(char *str)
{
    return (*str=='%')?TRUE:FALSE;
}

/**
 * This function checks if a given string is a valid machine instruction.
 * Returns TRUE if it matches an instruction name, FALSE otherwise.
 */
int is_instruction(char *str) 
{
    char *temp;
    int i = 0;
    while((temp = get_instruction_name(i++)) != NULL)
        if(!strcmp(str, temp))
            return TRUE;
    return FALSE;
}

/**
 * This function checks if a given string represents a valid register (r0-r7).
 * Returns TRUE if it is a register, FALSE otherwise.
 */
int is_register(char *str)
{
    if(strlen(str)!=2) return FALSE;
    return(str[0]=='r' && (str[1] >= '0' && str[1] <= '7'))?TRUE:FALSE;
}

/**
 * This general use function checks if a given string is a reserved word
 * of the assembler (instruction, register, directive, or macro keyword).
 * Returns TRUE if it is reserved, FALSE otherwise.
 */
int reserved(char *str)
{
    if(is_instruction(str) || is_register(str) || is_data(str) || is_entry(str) || is_extern(str) || is_string(str)) return TRUE;
    if(!strcmp(str, MACRO_START) || !strcmp(str, MACRO_END)) return TRUE;
    return FALSE;
}

/**
 * This general use function makes sure that a symbol is defined according 
 * to the correct rules (length, starting with a letter, alphanumeric characters,
 * not reserved, etc.). It reports an error if the symbol is invalid.
 */
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

/**
 * This function returns the addressing mode of a given operand string.
 * (Immediate, Relative, Register, or Direct).
 */
int get_mode(char *str)
{
    if(is_immediate(str)) return IMM;
    if(is_relative(str)) return REL;
    if(is_register(str)) return REG;
    return DIR;
}

/**
 * This function receives a string and checks if it is a valid integer number
 * that fits within the limits of a 12-bit word. 
 * Returns TRUE if the number is valid, FALSE otherwise.
 */
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

/**
 * This function receives an array of strings representing data integers,
 * and validates that each one is a valid number.
 * Returns TRUE if all are valid, FALSE otherwise.
 */
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

/**
 * This function receives a string and validates that it represents a correct
 * string literal (enclosed in double quotes).
 * Returns TRUE if valid, FALSE otherwise.
 */
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

/**
 * This function wraps different memory freeing functions.
 * It frees all symbol lists, macro lists, and closes the file pointer.
 */
void free_data(file_data *f)
{
    free_symbols(f->symbol_list);
    free_macros(f->macro_list);
    fclose(f->ptr);
}

   