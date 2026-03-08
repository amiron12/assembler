#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include "symbol_table.h"
#include <stdio.h>

typedef struct file_state
{
    char name[FILE_NAME];
    char extended_name[FILE_NAME];
    FILE *ptr;
    int current_line;
    int error_flag;
} file_state;

void init_file_state(file_state *fs, char *fname, char *ext);

void create_obj_file(char *file_name, symbol *symbol_table);

void create_ent_file(symbol *head, char *file_name);

void create_ext_file(symbol *head, char *file_name);

#endif