#ifndef MACHINE_IMAGE_H
#define MACHINE_IMAGE_H
#include "constants.h"
#include "structs.h"

#define OP_SHIFT 8
#define FUNCT_SHIFT 4
#define SRC_SHIFT 2

/* Encodes an instruction and its operands into the machine image */
void encode_instruction(char *str, char* src, char* dest, file_data *fs, char* file_name);

/* Encodes an array of numeric data arguments into the machine image */
void encode_data(char *args[], file_data* file);

/* Encodes a string literal into the machine image */
void encode_string(char *str, file_data* file);

#endif
