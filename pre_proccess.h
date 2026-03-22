
#ifndef PRE_PROCCESS_H
#define PRE_PROCCESS_H

#include "structs.h"

/* This function takes a .as file and creates an .am file after removing comments and empty lines, and expands the macros defined in it */
void expand_macros(file_data *fs, file_data *am_file);

#endif