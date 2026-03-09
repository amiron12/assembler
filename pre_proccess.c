
#include "pre_proccess.h"
#include "assembler.h"
#include "output.h"
#include "utils.h"
#include "text_parsing.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



static void append_content(macro_line *head, macro_line *new_line);
static void new_macro(char *name, macro **curr);
static void expand(char *macro_name, macro *macro_head, FILE *expanded_file);
static char *macro_start_check(char *line);
static int is_macro_call(char *str, macro *head);
static void add_macro_line(char *line, macro *curr);
static void free_macros(macro *macro_head);
static void free_lines(macro_line *head);

void expand_macros(file_state *as_file, file_state *am_file)
{
    macro *head;
    char buffer[MAX_LINE_LENGTH];
    int inside_macro = FALSE;
    head = NULL;    

    fprintf(stderr,"[INFO] starting pre-proccess\n");
    
    while(fgets(buffer, sizeof(buffer), as_file->ptr) != NULL)
    {
        /* TODO: check size is correct (with /0 maybe) */
        char *first_arg, *rest;
        char line[MAX_LINE_LENGTH];
        strcpy(line, buffer);
        as_file->current_line++;
        
        if(is_empty_line(line) || is_comment(line)) continue; /* TODO: avoid these lines? */


        first_arg = strtok(line, " \t\n\r");
        rest = strtok(NULL, "\n");
        
        /* Found a macro start */
        if(!strcmp(first_arg, MACRO_START)) 
        {
            char *macro_name;
            char *err = macro_start_check(rest);
            if(err != NULL)
            {
                error(as_file, err);
                free_macros(head);
                return;
            }
            inside_macro = TRUE;
            macro_name = strtok(rest, " \t");
            new_macro(macro_name, &head); /* creating a macro node */
            continue;
        }
        

        /* Found a macro end */
        if(!strcmp(first_arg, MACRO_END))
        {
            if(is_empty_line(rest)) /* checking that there is no text after macro end call */
            {
                inside_macro = FALSE;
                continue;
            }
            error(as_file, "Extraneous text after macro end statement");
            free_macros(head);
            return;
        }

        if(inside_macro)
            add_macro_line(buffer, head);
    
        else if(is_macro_call(first_arg, head))
            expand(first_arg, head, am_file->ptr);
    
        else
            fputs(buffer, am_file->ptr);
        
    }

    /* finished file */
    if(as_file->error_flag)
    {
        remove(am_file->extended_name);
        fprintf(stderr,"[INFO] %s deleted\n", as_file->extended_name);
        return;
    }
    free_macros(head);
    fclose(am_file->ptr);
    am_file->ptr = fopen(am_file->extended_name, "r");
    memory_check(am_file->ptr);
    fprintf(stderr,"[INFO] finished pre-proccess\n");

    
}

static void new_macro(char *name, macro **head)
{
    {
        macro *new_node = (macro *)malloc(sizeof(macro));
        memory_check(new_node);          
        strncpy(new_node->name, name, MAX_LINE_LENGTH);
        new_node->content = NULL;
        new_node->next = *head;
        *head = new_node;
    }    
}

static char *macro_start_check(char *line)
{
    char *macro_name;
    macro_name = strtok(line, " \t");
    line = strtok(NULL, "");
    if(macro_name == NULL)
        return "Macro name not specified";
    if(!not_reserved(macro_name))
        return "Macro name is a reserved word";
    if(!is_empty_line(line))
        return "Extraneous text after macro name";
    return NULL;
}


static void expand(char *macro_name, macro *macro_head, FILE *expanded_file)
{
    macro *ptr = macro_head;
    while(ptr != NULL && strcmp(ptr->name, macro_name))
        ptr = ptr->next;
    
    if (ptr) 
    {
        macro_line *curr_line = ptr->content;
        while (curr_line != NULL) 
        {
            fputs(curr_line->text, expanded_file);
            curr_line = curr_line->next;
        }
    }
}

static int is_macro_call(char *str, macro *head)
{
    macro *temp = head;
    while(temp != NULL)
    {
        if(!strcmp(temp->name, str))
            return TRUE;
        temp = temp->next;
    }
    return FALSE;
}

static void add_macro_line(char *line, macro *head)
{
    macro_line *new_line = (macro_line *)malloc(sizeof(macro_line));
    memory_check(new_line);
    strncpy(new_line->text, line, MAX_LINE_LENGTH);
    new_line->next=NULL;

    if(head->content == NULL)
        head->content = new_line;
    else
        append_content(head->content, new_line);
}

static void append_content(macro_line *head, macro_line *new_line)
{
    macro_line *temp;
    temp = head;
    while(temp->next != NULL)
        temp = temp->next;
    temp->next = new_line;
}

static void free_macros(macro *head)
{
    macro *temp = head;
    while(head != NULL)
    {
        temp = head;
        head = head->next;
        free_lines(temp->content);
        free(temp);
    }
    fprintf(stderr,"[INFO] Macro list free'd\n");

}

static void free_lines(macro_line *head)
{
    macro_line *temp = head;
    while(head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}
