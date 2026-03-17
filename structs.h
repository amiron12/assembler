#ifndef STRUCTS_H
#define STRUCTS_H

#include "constants.h"
#include <stdio.h>

typedef struct macro
{
    char name[LINE_LENGTH];
    struct macro *next; /*pointer to the next macro */
    struct macro_line *content; /*pointer to the first line of the linked list refering to this macro name*/
} macro;

typedef struct macro_line
{
    struct macro_line *next;
    char text[LINE_LENGTH];
} macro_line;

typedef struct symbol
{
    char name[SYMBOL_LENGTH];
    int address;
    unsigned int attr:ATTR;
    struct symbol *next;
} symbol;

typedef struct machine_word
{ 
    unsigned int word:WORD_SIZE;
    char type;
} machine_word;

typedef struct file_data
{
    char name[FILE_NAME];
    char extended_name[FILE_NAME];
    int current_line;
    int error_flag;
    FILE *ptr;
    macro *macro_list;
    symbol *symbol_list;
} file_data;

#endif
