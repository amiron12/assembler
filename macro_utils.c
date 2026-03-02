
#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void new_macro(char *name, macro **file_macros)
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


void expand(char *macro_name, macro *macro_head, code_file *expanded_file)
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

void add_macro_line(char *line, macro **file_macros)
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

void add_standard_line(char *line, code_file *expanded_file)
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

void cleanup(macro *macro_head, code_file *expanded_file)
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
