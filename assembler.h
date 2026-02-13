
#ifndef SET_H
#define SET_H
#define MEM_START 100
#define MAX_LINE_LENGTH 80 /* including new-line character */
#define MAX_FNAME 15
#define ZERO 0
#define ONE 1
#define OK 1
#define EXTENTION ".as"

typedef short word;


typedef struct Node
{
    char *name;
    FILE *start_ptr;
    FILE *end_ptr;
    struct Node *next;
} Node;




















#endif