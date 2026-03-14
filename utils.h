#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include "structs.h"

void get_next_word(char **line, char **word);

int reserved(char *str);

int get_mode(char *str);


int validate_data(char *args[]);

int validate_string(char *str);

void validate_symbol(char *str, file_data *fs);

int is_directive(char *str);
int is_immediate(char *str);
int is_relative(char *str);
int valid_symbol_format(char *str);
int is_instruction(char *str);
int is_register(char *str);
int is_string(char *str);
int is_data(char *str);
int is_extern(char *str);
int is_entry(char *str);

void clean_string(char **str);

void memory_check(void *ptr);

void file_check(FILE *fp);

void error(file_data *state, char *str);

void extention(file_data *fs, char *ext);

int is_macro_call(char *str, macro *head); /* defined in pre-proccess.c */
void free_macros(macro *head);

void abort_file(file_data *f);

/* TODO: delete: */
void print_machine_images();
void save_machine_images(char *name);
extern machine_word code_image[MEMORY];
extern machine_word data_image[MEMORY];

#endif