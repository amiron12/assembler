
#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MACRO_START "mcro"
#define MACRO_END "mcroend"
#define FILE_ERROR 1
#define TRUE 1
#define FALSE 0




macro *new_macro(char *name);
macroline *search_macro(char *name, macro *head);
macroline *add_line(macro *mac, char *line);

void expand_macros(FILE *fps, FILE *fpm)
{

    macro *head;
    macro *curr_macro;
    macroline *curr_line;
    char buffer[MAX_LINE_LENGTH];
    char *name;
    /*FILE *fps; file pointer for .as file*/
    /*FILE *fpm; file pointer for .am file*/
    /*char *fns; file name for .as ending*/
    /*char fnm[MAX_ARG_LENGTH]; file name for .am ending*/
    int inside_macro = FALSE;
    name = "head";
    head = new_macro(name);
    curr_macro = head;
    curr_line = NULL;
    
    
    
   /* fps = fps; */
   /* strcpy(fnm, fname);*/
    /* strcat(fns, EXT_AS);  adding the correct file ending */
   /* strcat(fnm, EXT_AM);  adding the correct file ending */
    

    /* fps = fopen(fns, "r");
    if(fps==NULL) return FILE_ERROR; */
    /*fpm = fopen(fnm, "w");
    if(fpm==NULL) return NULL; */
   

    while(fgets(buffer, sizeof(buffer), fps) != NULL)
    {
        /* TODO: check size is correct (with /0 maybe) */

        int count;
        char first_word[MAX_ARG_LENGTH], second_word[MAX_ARG_LENGTH], junk[MAX_ARG_LENGTH];
        count = sscanf(buffer, "%s %s %1s", first_word, second_word, junk);
        
        
        if(strcmp(first_word, MACRO_START)==0) /* first word of the line is 'mcro' */
        {
            macro *temp;
            if(count != 2)
                {
                    printf("macro statment incorrect\n");
                    exit(1);
                }
            inside_macro = TRUE;
            temp = new_macro(second_word); /* creating a macro node */
            curr_macro->next = temp;
            curr_macro = temp;
            curr_line = NULL;
            continue;
        }
        
        if(strcmp(first_word, MACRO_END)==0)
        {
            if(count != 1)
               return; /* macro end is followed by extra characters */
            inside_macro = FALSE;
            continue;
        }

        if(inside_macro)
        {       
                macroline *temp;
                temp = add_line(curr_macro, buffer);
                if(curr_macro->lines==NULL)
                    curr_macro->lines = temp;
                else
                {
                    curr_line->next = temp;
                }
                curr_line = temp;
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
    memory_check(new_line);
    strncpy(new_line->line, line, MAX_LINE_LENGTH);
    new_line->next=NULL;
    return new_line;
}

macro *new_macro(char *name)
{
    macro *new_node = (macro *)malloc(sizeof(macro));
    memory_check(new_node);
    if(is_reserved(name))
    {
        free(new_node);
        return NULL;
    }
    strncpy(new_node->name, name, MAX_LINE_LENGTH);
    new_node->lines = NULL;
    new_node->next = NULL;
    return new_node;
}


/* TODO: free memory function after the whole proccess */