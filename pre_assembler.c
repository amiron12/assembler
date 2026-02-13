
#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MACRO_START "mcro"
#define MACRO_END "mcroend"
#define FILE_ERROR 1
#define AM_EXTENSION ".am"
#define AS_EXTENSION ".as"


typedef struct Macroline
{
    char line[MAX_LINE_LENGTH];
    struct Macroline *next;
} Macroline;


typedef struct Macro
{
    char name[MAX_LINE_LENGTH];
    Macroline *lines; /*pointer to the first line of the linked list refering to this macro name*/
    struct Macro *next; /*pointer to the next macro */
} Macro;





int find_macros(FILE *fp, char *fname)
{

    int i;
    char buffer[MAX_LINE_LENGTH];
    Macro *curr = NULL;
    int inside_macro = ZERO;

    FILE *fps, *fpm;
    char *fns = fname;
    char *fnm = fname;
    strcat(fns, AS_EXTENSION); /* adding the correct file ending */
    strcat(fnm, AM_EXTENSION); /* adding the correct file ending */
    

    fps = fopen(fns, "r");
    if(fps==NULL) return FILE_ERROR; 
    fpm = fopen(fnm, "w");
    if(fpm==NULL) return FILE_ERROR; 


    while(fgets(buffer, sizeof(buffer), fps) != NULL)
    {
        char *first_word, *second_word;
        sscanf(buffer, "%s, %s", first_word, second_word);
        
        if(strcmp(first_word, MACRO_START))
        {
            /* TODO: check name is legit */
            new_macro(second_word);
            inside_macro = ONE;
        }
        
        if(strcmp(first_word, MACRO_END))
        {
            inside_macro = ZERO;
        }
        if(inside_macro == ONE)
        {

            

            else
                add_line(buffer);
            
        }

        if(inside_macro == ZERO)
            fputs(buffer, fpm);
        
        
    }






    return 0;
   
   
    /* TODO: check size is correct (with /0 maybe) */
}


Macroline *add_line(char *line)
{
    Macroline *new_line = (Macroline *)malloc(sizeof(Macroline));
    if(line == NULL)
    {
        printf("Memory alocation failed");
        exit(1); /* TODO: exit in this situation? */
    }
    strncpy(new_line->line, line, MAX_LINE_LENGTH);
    new_line->next=NULL;
    return new_line;

}

Macro *new_macro(char *name)
{
    Macro *new_node = (Macro *)malloc(sizeof(Macro));
    if(new_node == NULL)
    {
        printf("Memory alocation failed");
        exit(1); /* TODO: exit in this situation? */
    }

    strncpy(new_node->name, name, MAX_LINE_LENGTH);
    new_node->lines = NULL;
    new_node->next = NULL;
    return new_node;
}


int expand_macros(FILE *fp, char *fname)
{

}

