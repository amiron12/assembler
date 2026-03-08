
#include "pre_proccess.h"
#include "assembler.h"
#include "file_utils.h"
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
    code_file *exp_file;
    char buffer[MAX_LINE_LENGTH];
    int inside_macro = FALSE;

    exp_file = (code_file *)malloc(sizeof(code_file));
    exp_file->head = NULL;
    exp_file->tail = NULL;
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
                free_macros(file_macros, exp_file);
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
            if(is_empty_line(rest))
            {
                inside_macro = FALSE;
                continue;
            }
            error(as_file, "Extraneous text after macro end statement");
            free_macros(file_macros, exp_file);
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

    /* finished reading the file */
    create_am_file(as_file, &exp_file);
    free_macros(file_macros, exp_file);
}


static void create_am_file(file_state *as_file, code_file **expanded_file)
{
    FILE *am_file;
    code_line *curr;
    char *fname = as_file->name;
    fname = strcat(fname, AM);
    am_file = fopen(fname, "w+");
    file_check(am_file);
    curr = (*expanded_file)->head;
    while(curr != NULL)
    {
        fputs(curr->text, am_file);
        curr = curr->next;
    }
    fclose(am_file);
}


static void new_macro(char *name, macro **file_macros)
{
    {
        macro *new_node = (macro *)malloc(sizeof(macro));
        memory_check(new_node);          
        strncpy(new_node->name, name, MAX_LINE_LENGTH);
        new_node->content.head = NULL;
        new_node->content.tail = NULL;
        new_node->next = NULL;

        if(*file_macros == NULL) 
            *file_macros = new_node; /* first macro */
        else
            (*file_macros)->tail->next = new_node;

        (*file_macros)->tail = new_node;
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

static void add_standard_line(char *line, code_file *expanded_file)
{
    code_line *new_line = (code_line *)malloc(sizeof(code_line));
    memory_check(new_line);
    strncpy(new_line->text, line, MAX_LINE_LENGTH);
    new_line->next=NULL;

    


    if(expanded_file->head == NULL) /* first line */
        expanded_file->head = new_line;
    else
        expanded_file->tail->next = new_line;
    expanded_file->tail = new_line;
}

static void free_macros(macro *macro_head, code_file *expanded_file)
{
    macro *m_curr = macro_head;
    while (m_curr != NULL)
    {
        macro *m_next = m_curr->next;
        code_line *l_curr = m_curr->content.head;
        
        while (l_curr != NULL)
        {
            code_line *l_next = l_curr->next;
            free(l_curr);
            l_curr = l_next;
        }
        free(m_curr);
        m_curr = m_next;
    }

    if (expanded_file != NULL)
    {
        code_line *line_curr = expanded_file->head;
        while (line_curr != NULL)
        {
            code_line *line_next = line_curr->next;
            free(line_curr);
            line_curr = line_next;
        }
        free(expanded_file);
    }
}


