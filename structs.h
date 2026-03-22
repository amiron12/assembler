#ifndef STRUCTS_H
#define STRUCTS_H

#include "constants.h"
#include <stdio.h>

/* Represents a macro defined in the assembly source code */
typedef struct macro
{
    char name[LINE_LENGTH];
    struct macro *next; /*pointer to the next macro */
    struct macro_line *content; /*pointer to the first line of the linked list refering to this macro name*/
} macro;

/* Represents a single line of text within a macro definition */
typedef struct macro_line
{
    struct macro_line *next;
    char text[LINE_LENGTH];
} macro_line;

/* Represents a symbol (label) and its properties in the symbol table */
typedef struct symbol
{
    char name[SYMBOL_LENGTH];
    int address;
    unsigned int attr:ATTR;
    struct symbol *next;
} symbol;

/* Represents a single machine word (instruction or data) in the memory image */
typedef struct machine_word
{ 
    unsigned int word:WORD_SIZE;
    char type;
} machine_word;

/* Holds data related to the current file being processed, including state and data structures */
typedef struct file_data
{
    char name[MAX_FILE_NAME];
    char extended_name[MAX_FILE_NAME];
    int current_line;
    int error_flag;
    FILE *ptr;
    macro *macro_list;
    symbol *symbol_list;
} file_data;

#endif
