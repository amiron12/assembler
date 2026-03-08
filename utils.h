#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include "file_utils.h"
#include "symbol_table.h"



void memory_check(void *ptr);

void file_check(FILE *fp);

void error(file_state *state, char *str);

void extention(file_state *fs, char *ext);

#endif