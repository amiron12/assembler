#ifndef MACHINE_IMAGE_H
#define MACHINE_IMAGE_H
#include "constants.h"
#include "structs.h"

#define OP_SHIFT 8
#define FUNCT_SHIFT 4
#define SRC_SHIFT 2

machine_word data_image[MEMORY];
machine_word code_image[MEMORY];

extern machine_word code_image[MEMORY];
extern machine_word data_image[MEMORY];

/* Encodes an instruction and its operands into the machine image */
void encode_instruction(char *str, char* src, char* dest, file_data *fs);

/* Encodes an array of numeric data arguments into the machine image */
void encode_data(char *args[]);

/* Encodes a string literal into the machine image */
void encode_string(char *str);

#endif
