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


/* 
 * A node in the Symbol Table linked list.
 * Stores the symbol's name, resolved memory address, and a bitmask of its attributes.
 */
typedef struct symbol
{
    char name[SYMBOL_LENGTH];
    int address;
    unsigned int attr:ATTR;
    struct symbol *next;
} symbol;


/* 
 * Represents a single 12-bit machine word in the memory image.
 * The bit-field guarantees the logical structure is exactly 12 bits.
 * The 'type' field holds 'A', 'R', 'E' (Absolute, Relocatable, External), 
 * or '?' for unresolved addresses during Pass 1.
 */
typedef struct machine_word
{ 
    unsigned int word:WORD_SIZE;
    char type;
} machine_word;

/* Holds data related to the current file being processed, including state and data structures */
typedef struct file_data
{
    int current_line;
    int error_flag;
    macro *macro_list;
    symbol *symbol_list;
    int DC, IC, DCF, ICF;
    machine_word data_image[MEMORY];
    machine_word code_image[MEMORY];
} file_data;

#endif
