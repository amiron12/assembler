#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include "machine_image.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct code_line
{
    struct code_line *next;
    struct code_line *tail;
    char text[MAX_LINE_LENGTH];
} code_line;

typedef struct code_file
{
    struct code_line *head;
    struct code_line *tail;
} code_file;

typedef struct macro
{
    char name[MAX_LINE_LENGTH];
    struct macro *next; /*pointer to the next macro */
    struct code_file content; /*pointer to the first line of the linked list refering to this macro name*/
    struct macro *tail;
} macro;



int not_reserved(char *str);



int is_directive(char *str);
int is_immediate(char *str);
int is_relative(char *str);
int is_label(char *str);
int is_instruction(char *str);
int is_register(char *str);
int is_string(char *str);
int is_data(char *str);
int is_extern(char *str);
int is_entry(char *str);

void clean_string(char **str);


op_mode get_mode(char *str);


int validate_data(char *args[]);

int validate_string(char *str);










char *macro_start_check(char *line);
void expand(char *macro_name, macro *macro_head, code_file *expanded_file);
int macro_call(char *str, macro *head);
void add_macro_line(char *line, macro **curr_macro);
void add_standard_line(char *line, code_file *expanded_file);
void new_macro(char *name, macro **curr_macro);
void cleanup(macro *macro_head, code_file *expanded_file);







line_type sentence_type(char *str);
attribute dir_type(char *str);


/* TODO: delete: */
void print_machine_images();
void save_machine_images(char *name);
extern machine_word code_image[MEMORY];
extern machine_word data_image[MEMORY];

#endif