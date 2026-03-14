#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include "structs.h"

void add_symbol(char *symbol_name, symbol **curr,symbol **head, int address, attribute atr);

int symbol_exists(char *name, symbol *head);

symbol* get_symbol(char *name, symbol *head);

void free_symbols(symbol *head);



/* TODO: delete: */
void print_symbols(symbol *head);
void print_symbol_table(symbol *head);
void save_symbol_table(symbol *head, char *name);

#endif
