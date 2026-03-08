
#ifndef PRE_PROCCESS_H
#define PRE_PROCCESS_H
#include "constants.h"
#include "assembler.h"
#include "output.h"

typedef struct macro
{
    char name[MAX_LINE_LENGTH];
    struct macro *next; /*pointer to the next macro */
    struct macro_line *content; /*pointer to the first line of the linked list refering to this macro name*/
} macro;

typedef struct macro_line
{
    struct macro_line *next;
    char text[MAX_LINE_LENGTH];
} macro_line;


void expand_macros(file_state *fs);

#endif