#ifndef PASSES_H
#define PASSES_H    
#include "symbol_table.h"

symbol* start_pass(file_state *fs);

symbol* second_pass(file_state *fs, symbol *head);

#endif