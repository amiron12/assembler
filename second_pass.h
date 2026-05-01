#ifndef SECOND_PASS_H
#define SECOND_PASS_H
#include "structs.h"

/* This function performs the second pass of the assembler on a given file, and creates the needed output files */
void second_pass(file_data *fs, char* fname, char* am_extended_name);

#endif
