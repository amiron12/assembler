#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include "structs.h"

int reserved(char *str);

int get_mode(char *str);

int validate_number(char *str);
int validate_data(char *args[], file_data *fs);

int validate_string(char *str, file_data *fs);

void validate_symbol(char *str, file_data *fs);

int is_directive(char *str);
int is_immediate(char *str);
int is_relative(char *str);

int is_instruction(char *str);
int is_register(char *str);
int is_string(char *str);
int is_data(char *str);
int is_extern(char *str);
int is_entry(char *str);

void memory_check(void *ptr);

void error(file_data *state, char *str);

int is_macro_call(char *str, macro *head); /* defined in pre-proccess.c */

void free_macros(macro *head);
void free_data(file_data *f);


#endif