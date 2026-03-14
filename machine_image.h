#ifndef MACHINE_IMAGE_H
#define MACHINE_IMAGE_H
#include "constants.h"
#include "structs.h"

#define OP_SHIFT 8
#define FUNCT_SHIFT 4
#define SRC_SHIFT 2



machine_word data_image[MEMORY];
machine_word code_image[MEMORY];


void encode_instruction(char *str, int src, int dest);

void encode_operand(char *str);

void encode_data(char *args[]);

void encode_string(char *str);

#endif
