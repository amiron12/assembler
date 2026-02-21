
#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>




char *macro_start_check(char *line)
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


void expand(char *macro_name, macro *macro_head, code_line **curr_line)
{
    macro *ptr = macro_head;
    while(ptr != NULL)
    {
        if(strcmp(ptr->name, macro_name))
            break;
        ptr = ptr->next;
    }
    (*curr_line)->next = ptr->lines;
    (*curr_line) = ptr->lines;
}

int macro_call(char *str, macro *head)
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

void add_macro_line(char *line, macro **curr_macro, code_line **curr_line)
{
    code_line *new_line = (code_line *)malloc(sizeof(code_line));
    memory_check(new_line);
    strncpy(new_line->line, line, MAX_LINE_LENGTH);
    new_line->next=NULL;
    if(*curr_line==NULL)
        (*curr_macro)->lines = new_line;
    else
        (*curr_line)->next = new_line;
    *curr_line = new_line;
}

void add_standard_line(char *line, code_line **curr, file_state *state)
{
    code_line *new_line = (code_line *)malloc(sizeof(code_line));
    memory_check(new_line);
    strncpy(new_line->line, line, MAX_LINE_LENGTH);
    new_line->next=NULL;
    (*curr)->next = new_line;
    (*curr) = new_line;
}

void new_macro(char *name, macro **curr_macro)
{
    {
        macro *new_node = (macro *)malloc(sizeof(macro));
        memory_check(new_node);
        strncpy(new_node->name, name, MAX_LINE_LENGTH);
        new_node->lines = NULL;
        new_node->next = NULL;
        (*curr_macro)->next = new_node;
        (*curr_macro) = new_node;
    }    
}

void cleanup(macro *macro_head, code_line *lines_head)
{
    macro *temp = macro_head;
    code_line *curr = lines_head;
    while(temp != NULL)
    {
        macro *next = temp->next;
        free(temp);
        temp = next;
    }
    while(curr != NULL)
    {
        code_line *next = curr->next;
        free(curr);
        curr = next;
    }
}
