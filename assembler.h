
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef SET_H
#define SET_H
#define MEM_START 100
#define MAX_LINE_LENGTH 80 /* including new-line character */
#define MAX_FNAME 15
#define ZERO 0
#define ONE 1
#define OK 1
#define EXT_AS ".as"
#define EXT_AM ".am"
#define MAX_ARG_LENGTH 30


typedef struct Node
{
    char *name;
    FILE *start_ptr;
    FILE *end_ptr;
    struct Node *next;
} Node;


char *get_name(int index);

void expand_macros(FILE *fps, FILE *fpm);
void start_pass(FILE *fp);














#endif