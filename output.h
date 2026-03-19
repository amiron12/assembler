#ifndef OUTPUT_H
#define OUTPUT_H

#include "structs.h"
#include <stdio.h>


int init_output_files(file_data *fs);

void delete_output_files(char *file_name);

void delete_ent_file(char *file_name);

void delete_ext_file(char *file_name);

void delete_obj_file(char *file_name);

int append_entry(symbol *entry, file_data *fs);

int append_external(symbol *external, file_data *fs);

void init_file_data(file_data *fs, char *fname, char *ext, char *mode);

void create_obj_file(file_data *fs);

#endif