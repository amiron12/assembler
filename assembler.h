
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef SET_H
#define SET_H
#define BYTE (sizeof(unsigned char)*8)
#define MEM_START 100
#define MAX_LINE_LENGTH 80 /* including new-line character */
#define MAX_FNAME 15
#define ZERO 0
#define ONE 1
#define FALSE 0
#define TRUE 1
#define OK 1
#define EXT_AS ".as"
#define EXT_AM ".am"
#define MAX_ARG_LENGTH 30
#define LABEL_LENGTH 31
#define MEMORY 4096
#define WORD_SIZE 12

extern int DC, IC;

typedef enum attribute {DATA, CODE, EXTERNAL, ENTRY} attribute;


typedef struct machine_word
{ 
  unsigned int word : WORD_SIZE;
  char type;
} machine_word;


typedef struct symbol
{
    char name[LABEL_LENGTH];
    int address;
    enum attribute atr;
    struct symbol *next;
} symbol;

typedef struct macroline
{
    struct macroline *next;
    char line[MAX_LINE_LENGTH];
} macroline;

typedef struct macro
{
    char name[MAX_LINE_LENGTH];
    struct macro *next; /*pointer to the next macro */
    struct macroline *lines; /*pointer to the first line of the linked list refering to this macro name*/
} macro;


char *get_instruction_name(int index);

void memory_check(void *ptr);
void file_check(FILE *fp);

void expand_macros(FILE *fps, FILE *fpm);

void start_pass(FILE *fp);

void add_symbol(char *symbol_name, symbol **curr, int address, attribute atr);
void print_symbols(symbol *head);

void start(FILE *fp);

int tokenize(char *line, char *args[]);

int not_reserved(char *str);

int label_exist(char *name, symbol *head);

int is_directive(char *str);
int is_immediate(char *str);
int is_relative_label(char *str);
static int is_label(char *str);
int is_instruction(char *str);
int is_register(char *str);
int is_string(char *str);
int is_data(char *str);
int is_extern(char *str);
int is_entry(char *str);

char* clean_label(char *str);







#endif