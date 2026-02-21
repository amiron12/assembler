
#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MACRO_START "mcro"
#define MACRO_END "mcroend"

static void create_file(file_state *as_file, code_line *lines);

void expand_macros(file_state *as_file)
{
    FILE *fps;
    macro *macro_head, *curr_macro;
    code_line *lines_head, *curr_macro_line, *curr_line;
    char buffer[MAX_LINE_LENGTH];
    int inside_macro = FALSE;

    macro_head = (macro *)malloc(sizeof(macro));
    memory_check(macro_head);
    macro_head->next = NULL;
    macro_head->lines = NULL;

    lines_head = (code_line *)malloc(sizeof(code_line));
    memory_check(lines_head);
    lines_head->next = NULL;

    curr_line = lines_head;
    curr_macro = macro_head;



    fps = as_file->ptr; /* was opened in main */

    while(fgets(buffer, sizeof(buffer), fps) != NULL)
    {
        /* TODO: check size is correct (with /0 maybe) */
        char *arg, *rest;
        char line[MAX_LINE_LENGTH];
        strcpy(line, buffer);

        arg = strtok(line, " \t");
        rest = strtok(NULL, "\n");

        /* Found a macro start */
        if(!strcmp(arg, MACRO_START)) 
        {
            char *name;
            char *err = macro_start_check(rest);
            if(err != NULL)
            {
                error(as_file, err);
                cleanup(macro_head, lines_head);
                return;
            }

            inside_macro = TRUE;
            name = strtok(rest, " \t");
            new_macro(name, &curr_macro); /* creating a macro node */
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
            cleanup(macro_head, lines_head);
            return;
        }

        /* standard line */
        switch (inside_macro)
        {
            case TRUE: /* link line to the macro list */
            {
                add_macro_line(buffer, &curr_macro, &curr_macro_line);
                break;
            }

            case FALSE: /* link line to the standard list */
            {

                switch (macro_call(arg, macro_head))
                {
                case TRUE: /* macro reference */
                    expand(arg, macro_head, &curr_line);
                    break;
                
                case FALSE: /* regular line */
                    add_standard_line(buffer, &lines_head ,&curr_line, as_file);
                    break;
                }
            }
        }       
    }

    /* finished reading the file */
    


    print_machine_images();
    create_file(as_file, lines_head);
    cleanup(macro_head, lines_head);
}


static void create_file(file_state *as_file, code_line *lines)
{
    FILE *am_file;
    code_line *curr;
    char *fname = as_file->name;
    fname = strcat(fname, EXT_AM);
    am_file = fopen(fname, "w+");
    file_check(am_file);
    curr = lines;
    while(curr != NULL)
    {
        fputs(curr->line, am_file);
        curr = curr->next;
    }
    fclose(am_file);
}


