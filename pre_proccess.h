
#ifndef PRE_PROCCESS_H
#define PRE_PROCCESS_H
#include "constants.h"
#include "assembler.h"
#include "file_utils.h"

typedef struct macro
{
    char name[MAX_LINE_LENGTH];
    struct macro *next; /*pointer to the next macro */
    struct code_file content; /*pointer to the first line of the linked list refering to this macro name*/
    struct macro *tail;
} macro;


void expand_macros(file_state *fs);

#endif