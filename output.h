#ifndef OUTPUT_H
#define OUTPUT_H

#include "structs.h"
#include <stdio.h>


void init_output_files(char *file_name);

void delete_output_files(char *file_name);

void append_entry(symbol *entry, char *file_name);

void append_external(symbol *external, char *file_name);

void init_file_state(file_data *fs, char *fname, char *ext, char *mode);

void create_obj_file(char *file_name, symbol *symbol_table);

#endif