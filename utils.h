#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include "structs.h"

/* Initializes the file_data structure with the given file name, extension, and mode */
void init_file_data(file_data *fs, char *fname, char *ext, char *mode);

/* Checks if a given string is a reserved keyword */
int reserved(char *str);

/* returns the addressing mode of an operand */
int get_mode(char *str);

/* Checks if a string represents a valid integer within limits */
int validate_number(char *str);

/* Validates an array of data integers */
int validate_data(char *args[], file_data *fs);

/* Validates that a string is properly formatted */
int validate_string(char *str, file_data *fs);

/* Validates the naming rules and constraints for a symbol */
void validate_symbol(char *str, file_data *fs);

/* Checks if a string represents a directive (starts with a dot) */
int is_directive(char *str);

/* Checks if an operand is an immediate value */
int is_immediate(char *str);

/* Checks if an operand is a relative address */
int is_relative(char *str);

/* Checks if a string is a valid machine instruction */
int is_instruction(char *str);

/* Checks if a string represents a valid register */
int is_register(char *str);

/* Checks if a string is the ".string" directive */
int is_string(char *str);

/* Checks if a string is the ".data" directive */
int is_data(char *str);

/* Checks if a string is the ".extern" directive */
int is_extern(char *str);

/* Checks if a string is the ".entry" directive */
int is_entry(char *str);

/* Validates successful memory allocation, exiting on failure */
void memory_check(void *ptr);

/* prints an error message and flags the current file with an error */
void error(file_data *state, char *str);

/* Checks if a string matches a defined macro name */
int is_macro_call(char *str, macro *head); /* defined in pre-proccess.c */

/* Frees memory associated with the macro linked list */
void free_macros(macro *head);

/* Frees memory for symbols, macros, and closes the file pointer */
void free_data(file_data *f);


#endif