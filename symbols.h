#ifndef SYMBOLS_H
#define SYMBOLS_H
#include "structs.h"

void add_symbol(char *symbol_name, symbol **head, int address, attribute atr);

int symbol_exists(char *name, symbol *head);

symbol* get_symbol(char *name, symbol *head);

void free_symbols(symbol *head);

int is_attribute(symbol *sym, attribute attr);

void set_attribute(symbol *sym, attribute attr);



/* TODO: delete: */
void print_symbols(symbol *head);
void print_symbol_table(symbol *head);
void save_symbol_table(symbol *head, char *name);

#endif
