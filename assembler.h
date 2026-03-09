#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include "machine_image.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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


int get_mode(char *str);


int validate_data(char *args[]);

int validate_string(char *str);

line_type sentence_type(char *str);


/* TODO: delete: */
void print_machine_images();
void save_machine_images(char *name);
extern machine_word code_image[MEMORY];
extern machine_word data_image[MEMORY];

#endif