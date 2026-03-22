#ifndef OUTPUT_H
#define OUTPUT_H

#include "structs.h"
#include <stdio.h>

/* Deletes all generated output files (.ent, .ext, .ob) for a given base file name */
void delete_output_files(char *file_name);

/* Deletes the entries (.ent) file for a given base file name */
void delete_ent_file(char *file_name);

/* Deletes the externals (.ext) file for a given base file name */
void delete_ext_file(char *file_name);

/* Deletes the object (.ob) file for a given base file name */
void delete_obj_file(char *file_name);

/* Appends an entry symbol and its resolved address to the entries (.ent) output file */
int append_entry(symbol *entry, file_data *fs);

/* Appends an external symbol and the address where it is used to the externals (.ext) output file */
int append_external(symbol *external, file_data *fs);

/* Initializes the output files for the second pass */
int init_output_files(file_data *fs);

/* Generates the final object (.ob) file, containing the instruction and data counters, and the memory image */
void create_obj_file(file_data *fs);

#endif