/*
 * pre_proccess.c
 * This file handles the pre-processing stage (macro expansion) of the assembler. 
 * It reads the source code, identifies and stores macro definitions, and expands 
 * macro calls by replacing them with their stored content, resulting in an 
 * expanded source file (.am) for the first pass.
 */

#include "pre_proccess.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string_utils.h"
#include "constants.h"
#include "structs.h"
#include "utils.h"


static void append_content(macro_line *head, macro_line *new_line);
static void new_macro(char *name, macro **curr);
static void expand(char *macro_name, macro *head, FILE *am_file);
static char *validate_macro_start(char *macro_name, char *line);
static void add_macro_line(char *line, macro *curr);
static void free_lines(macro_line *head);

/**
 * This function handles the macro expansion stage (pre-assembler).
 * It reads the source file (.as), identifies macro definitions, and 
 * stores them. When a macro is called, it expands the macro by writing
 * its content to the expanded source file (.am).
 */
void expand_macros(file_data *as_file, file_data *am_file)
{
    macro *head;
    char buffer[LINE_LENGTH];
    int inside_macro = FALSE;
    head = NULL;   
    
    while(fgets(buffer, LINE_LENGTH, as_file->ptr) != NULL)
    {
        char *first_arg, *line;
        char buff_copy[LINE_LENGTH];
        strcpy(buff_copy, buffer);
        as_file->current_line++;
        line = buff_copy;
        
        if(is_empty_line(line) || is_comment(line)) continue; /* avoiding empty and comment lines */

        get_next_word(&line, &first_arg);
        
        /* Found a macro start */
        if(!strcmp(first_arg, MACRO_START)) 
        {
            char *macro_name, *err;
            get_next_word(&line, &macro_name);
            err = validate_macro_start(macro_name, line);
            if(err != NULL)
            {
                error(as_file, err);
                break;
            }
            inside_macro = TRUE;
            new_macro(macro_name, &head); /* creating a macro node */
            continue;
        }

        /* Found a macro end */
        if(!strcmp(first_arg, MACRO_END))
        {
            if(is_empty_line(line)) /* checking that there is no text after macro end call */
            {
                inside_macro = FALSE;
                continue;
            }
            error(as_file, "Extraneous text after macro end statement");
            break;
        }

        if(inside_macro)
            add_macro_line(buffer, head);
    
        else if(is_macro_call(first_arg, head))
            expand(first_arg, head, am_file->ptr);
    
        else
            fputs(buffer, am_file->ptr);
        
    }

    /* finished file */
    fclose(am_file->ptr);
    fclose(as_file->ptr);

    if(as_file->error_flag)
        remove(am_file->extended_name); /* error in the source file, .am file not created */

    am_file->macro_list = head;
    am_file->ptr = fopen(am_file->extended_name, "r"); /* opening in reading mode for first pass */
    if(!(am_file->ptr))
        error(am_file, "Error opening file");
}

/**
 * This helper function allocates memory for a new macro and inserts it 
 * at the beginning of the macro linked list.
 */
static void new_macro(char *name, macro **head)
{
    {
        macro *new_node = (macro *)malloc(sizeof(macro));
        memory_check(new_node);          
        strncpy(new_node->name, name, LINE_LENGTH);
        new_node->content = NULL;
        new_node->next = *head;
        *head = new_node;
    }    
}

/**
 * This helper function validates the macro name and checks for any extraneous
 * text after the macro definition. Returns an error message string if invalid, 
 * or NULL if valid.
 */
static char *validate_macro_start(char *macro_name, char *line)
{
    if(macro_name == NULL)
        return "Macro name not specified";
    if(reserved(macro_name))
        return "Macro name is a reserved word";
    if(!is_empty_line(line))
        return "Extraneous text after macro name";
    return NULL;
}

/**
 * This helper function expands a macro by finding it in the macro list
 * and writing its stored content to the output file (.am).
 */
static void expand(char *macro_name, macro *head ,FILE *am_file)
{
    macro *ptr = head;
    while(ptr != NULL && strcmp(ptr->name, macro_name))
        ptr = ptr->next;
    
    if (ptr) 
    {
        macro_line *curr_line = ptr->content;
        while (curr_line != NULL) 
        {
            fputs(curr_line->text, am_file);
            curr_line = curr_line->next;
        }
    }
}

/**
 * This function checks if a given string matches any of the stored macro names.
 * Returns TRUE if it is a macro call, FALSE otherwise.
 */
int is_macro_call(char *str, macro *head)
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

/**
 * This helper function creates a new line of text for a macro and appends 
 * it to the content list of the current macro.
 */
static void add_macro_line(char *line, macro *head)
{
    macro_line *new_line = (macro_line *)malloc(sizeof(macro_line));
    memory_check(new_line);
    strncpy(new_line->text, line, LINE_LENGTH);
    new_line->next=NULL;

    if(head->content == NULL)
        head->content = new_line;
    else
        append_content(head->content, new_line);
}

/**
 * This helper function traverses the macro's content linked list and appends 
 * a newly created line to the end.
 */
static void append_content(macro_line *head, macro_line *new_line)
{
    macro_line *temp;
    temp = head;
    while(temp->next != NULL)
        temp = temp->next;
    temp->next = new_line;
}

/**
 * This function frees all memory associated with the macro linked list, 
 * including their content lines.
 */
void free_macros(macro *head)
{
    macro *temp = head;
    while(head != NULL)
    {
        temp = head;
        head = head->next;
        free_lines(temp->content);
        free(temp);
    }
}

/**
 * This helper function frees the memory allocated for the macro lines
 * within a macro's content list.
 */
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
