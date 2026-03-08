#ifndef OUTPUT_H
#define OUTPUT_H

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

void init_output_files(char *file_name);

void delete_output_files(char *file_name);

void append_entry(symbol *entry, char *file_name);

void append_external(symbol *external, char *file_name);

void init_file_state(file_state *fs, char *fname, char *ext, char *mode);

void create_obj_file(char *file_name, symbol *symbol_table);

#endif