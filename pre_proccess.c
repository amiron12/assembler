
#include "pre_proccess.h"
#include "assembler.h"
#include "output.h"
#include "utils.h"
#include "text_parsing.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



static void create_am_file(file_state *as_file, code_file **lines);
static void new_macro(char *name, macro **file_macros);
static char *macro_start_check(char *line);
static void expand(char *macro_name, macro *macro_head, code_file *expanded_file);
static int is_macro_call(char *str, macro *head);
static void add_macro_line(char *line, macro **file_macros);
static void add_standard_line(char *line, code_file *expanded_file);
static void free_macros(macro *macro_head, code_file *expanded_file);

void expand_macros(file_state *as_file)
{
    macro *file_macros;
    char buffer[MAX_LINE_LENGTH];
    int inside_macro = FALSE;
    file_macros = NULL;

    while(fgets(buffer, sizeof(buffer), as_file->ptr) != NULL)
    {
        /* TODO: check size is correct (with /0 maybe) */
        char *arg, *rest;
        char line[MAX_LINE_LENGTH];
        strcpy(line, buffer);
        as_file->current_line++;
        
        if(is_empty_line(line))
            goto standard_line; /* TODO: change */
        arg = strtok(line, " \t\n\r");
        rest = strtok(NULL, "\n");
        
        /* Found a macro start */
        if(!strcmp(arg, MACRO_START)) 
        {
            char *macro_name;
            char *err = macro_start_check(rest);
            if(err != NULL)
            {
                error(as_file, err);
                /* TODO: freemacros */
                return;
            }

            inside_macro = TRUE;
            macro_name = strtok(rest, " \t");
            new_macro(macro_name, &file_macros); /* creating a macro node */
            continue;
        }
        
        /* Found a macro end */
        if(!strcmp(arg, MACRO_END))
        {
            if(is_empty_line(rest)) /* checking that there is no text after macro end call */
            {
                inside_macro = FALSE;
                continue;
            }
            error(as_file, "Extraneous text after macro end statement");
            /* TODO: freemacros */
            return;
        }

        standard_line:
        if(inside_macro)
            add_macro_line(buffer, &file_macros);
        else
            if(is_macro_call(arg, file_macros))
                expand(arg, file_macros, exp_file);
            else
                add_standard_line(buffer, exp_file);
    }
}




static void new_macro(char *name, macro *curr)
{
    {
        macro *new_node = (macro *)malloc(sizeof(macro));
        memory_check(new_node);          
        strncpy(new_node->name, name, MAX_LINE_LENGTH);
        new_node->next = NULL;
        new_node->content = NULL;


        if(head == NULL) 
            head = new_node; /* first macro */
        else
            head = new_node;

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


static void expand(char *macro_name, macro *macro_head, code_file *expanded_file)
{
    macro *ptr = macro_head;
    while(ptr != NULL && strcmp(ptr->name, macro_name) != 0)
        ptr = ptr->next;
    
    if (ptr) 
    {
        code_line *curr = ptr->content.head;
        while (curr != NULL) 
        {
            add_standard_line(curr->text, expanded_file); 
            curr = curr->next;
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

static void add_macro_line(char *line, macro **file_macros)
{

    macro *curr_macro = (*file_macros)->tail;
    code_line *new_line = (code_line *)malloc(sizeof(code_line));
    memory_check(new_line);
    strncpy(new_line->text, line, MAX_LINE_LENGTH);
    new_line->next=NULL;


    if(curr_macro->content.head == NULL)
        curr_macro->content.head = new_line;
    else
        curr_macro->content.tail->next = new_line;
    curr_macro->content.tail = new_line;
}

