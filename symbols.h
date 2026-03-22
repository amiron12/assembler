#ifndef SYMBOLS_H
#define SYMBOLS_H
#include "structs.h"

/* Allocates memorry and adds a new symbol to the beginning of the symbol list */
void add_symbol(char *symbol_name, symbol **head, int address, attribute atr);

/* Checks if a symbol with the given name exists in the list */
int symbol_exists(char *name, symbol *head);

/* Retrieves a symbol node by its name from the list */
symbol* get_symbol(char *name, symbol *head);

/* Frees all memory allocated for the symbol list */
void free_symbols(symbol *head);

/* Checks if a symbol possesses a specific attribute */
int is_attribute(symbol *sym, attribute attr);

/* Turns on a specific attribute bit for a symbol */
void set_attribute(symbol *sym, attribute attr);

#endif
