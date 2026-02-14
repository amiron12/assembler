
#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MACRO_START "mcro"
#define MACRO_END "mcroend"
#define FILE_ERROR 1



typedef struct
{
    char line[MAX_LINE_LENGTH];
    struct macroline *next;
} macroline;


typedef struct
{
    char name[MAX_LINE_LENGTH];
    macroline *lines; /*pointer to the first line of the linked list refering to this macro name*/
    struct macro *next; /*pointer to the next macro */
} macro;





int expand_macros(FILE *fp, char *fname)
{

    macro *head = new_macro("head");
    macro *curr_macro = head;
    macroline *curr_line = NULL;
    char buffer[MAX_LINE_LENGTH];
    int inside_macro = ZERO;

    FILE *fps = fp; /*file pointer for .as file*/
    FILE *fpm; /*file pointer for .am file*/
    char *fns = fname; /*file name for .as ending*/
    char *fnm = fname; /*file name for .am ending*/
    strcat(fns, AS_EXTENSION); /* adding the correct file ending */
    strcat(fnm, AM_EXTENSION); /* adding the correct file ending */
    

    fps = fopen(fns, "r");
    if(fps==NULL) return FILE_ERROR; 
    fpm = fopen(fnm, "w");
    if(fpm==NULL) return FILE_ERROR; 


    while(fgets(buffer, sizeof(buffer), fps) != NULL)
    {
        /* TODO: check size is correct (with /0 maybe) */
        char *first_word, *second_word;
        sscanf(buffer, "%s, %s", first_word, second_word);
        

        if(strcmp(first_word, MACRO_START)==0) /* first word of the line is 'mcro' */
        {
            macro *temp = new_macro(second_word); /* creating a macro node */
            curr_macro->next = temp;
            curr_macro = temp;
            curr_macro->lines = curr_line;
            inside_macro = ONE; /* flag representing the current macro state */
            continue;
        }
        
        if(strcmp(first_word, MACRO_END)==0)
        {
            inside_macro = ZERO;
            curr_macro = NULL;
            continue;
        }

        if(inside_macro)
        {
            if(curr_line==NULL)
                curr_line = add_line(curr_macro, buffer); /* first line of the macro */
            else
                {
                    macroline *temp;
                    temp = add_line(curr_macro, buffer);
                    curr_line->next = temp;
                    curr_line = curr_line->next;
                }

            continue;
        }

        else if(!inside_macro)
        {
            macroline *line_ptr = search_macro(first_word, head);
            if(line_ptr != NULL) 
               {
                    while(line_ptr != NULL) 
                    {
                        fputs(line_ptr->line, fpm); /* expanding a macro */
                        line_ptr = line_ptr->next;
                    }
                }
            else
                fputs(buffer, fpm); /* regular line to copy */

            continue;
        }       
    }
    return 0; /* returning 0 if the file is expanded and saved succesfully */  
}

macroline *search_macro(char *name, macro *head)
{
    macro *ptr = head;
    while(ptr != NULL)
    {
        if(strcmp(ptr->name, name)==0)
            return ptr->lines;
        ptr = ptr->next;
    }
    return NULL;
}


macroline *add_line(macro *mac, char *line)
{
    macroline *new_line = (macroline *)malloc(sizeof(macroline));
    if(line == NULL) /* TODO: make a generic global memory check */
    {
        printf("Memory alocation failed");
        exit(1); /* TODO: exit in this situation? */
    }
    strncpy(new_line->line, line, MAX_LINE_LENGTH);
    new_line->next=NULL;
    return new_line;
}

macro *new_macro(char *name)
{
    verify_name(name);
    macro *new_node = (macro *)malloc(sizeof(macro));
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


void verify_name(char *name)
{
    int i;
    i = 0;
    while(instructions[i].name != NULL)
    {
        if(strcmp(name, instructions[i].name)==0)
            {
                printf("Macro name cannot be an instruction's name");
                exit(1);
            }
    }
    /* TODO: refine checks */
}


