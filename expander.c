
#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MACRO_START "mcro"
#define MACRO_END "mcroend"

static void create_file(file_state *as_file, code_file **lines);

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

        arg = strtok(line, " \t\n\r");
        rest = strtok(NULL, "\n");
        
        /* Found a macro start */
        if(!strcmp(arg, MACRO_START)) 
        {
            char *name;
            char *err = macro_start_check(rest);
            if(err != NULL)
            {
                error(as_file, err);
                cleanup(file_macros, exp_file);
                return;
            }

            inside_macro = TRUE;
            name = strtok(rest, " \t");
            new_macro(name, &file_macros); /* creating a macro node */
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
            cleanup(file_macros, exp_file);
            return;
        }

        /* standard line */
        if(inside_macro)
            add_macro_line(buffer, &file_macros);
        else
            if(macro_call(arg, file_macros))
                expand(arg, file_macros, exp_file);
            else
                add_standard_line(buffer, exp_file);
    }

    /* finished reading the file */
    create_file(as_file, &exp_file);
    cleanup(file_macros, exp_file);
}


static void create_file(file_state *as_file, code_file **expanded_file)
{
    FILE *am_file;
    code_line *curr;
    char *fname = as_file->name;
    fname = strcat(fname, EXT_AM);
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


