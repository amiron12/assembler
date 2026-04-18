/*
 * symbols.c
 * This file handles symbol table management for the assembler. It provides 
 * functions to add new symbols, search for existing symbols, verify symbol 
 * attributes (such as data, code, entry, external), and manage memory for 
 * the symbol linked list.
 */

#include "symbols.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "constants.h"
#include "utils.h"

/**
 * This function receives a symbol list and all information for a new symbol.
 * It allocates a new symbol node, sets its information, and pushes it 
 * to the head of the list.
 */
void add_symbol(char *symbol_name, symbol **head, int address, attribute attr)
{
    symbol *new_node;
    if(symbol_name == NULL) return;
    new_node = (symbol *)malloc(sizeof(symbol));
    memory_check(new_node);
    strncpy(new_node->name, symbol_name, SYMBOL_LENGTH);
    new_node->address = address;
    (new_node->attr) = (1<<attr); /* turns on the required bit */
    new_node->next = *head;
    *head = new_node;
}

/**
 * This function receives a string and a symbol list.
 * It checks if there is a symbol with the given string as its name.
 * Returns the symbol pointer if it exists, otherwise returns NULL.
 */
symbol* get_symbol(char *name, symbol *head)
{
    symbol *curr = head;
    while(curr != NULL)
    {
        if(!strcmp(curr->name, name))
            return curr;
        curr = curr->next;
    }
    return NULL;
}

/**
 * This function receives a string and a symbol list.
 * It checks if there is a symbol with the given string as its name.
 * Returns TRUE if it exists, FALSE otherwise.
 */
int symbol_exists(char *name, symbol *head)
{
    if(get_symbol(name, head) == NULL)
        return FALSE;
    return TRUE;
}

/**
 * This function receives a symbol and an attribute and checks if the symbol 
 * holds this attribute. Returns a non-zero value if the attribute is present, 
 * otherwise returns zero.
 */
int is_attribute(symbol *sym, attribute attr)
{
    int val = 0;
    val = (1<<attr);
    return (sym->attr) & val;
}

/**
 * This function gets a symbol node and an attribute.
 * It turns on the bit corresponding to this attribute without deleting 
 * other attributes.
 */
void set_attribute(symbol *sym, attribute attr)
{
    (sym->attr) |= (1<<attr);
}

/**
 * This function traverses the given symbol list and frees the memory 
 * allocated for each symbol node.
 */
void free_symbols(symbol *head)
{
    symbol *temp;
    while(head != NULL)
    {   
        temp = head;
        head = head->next;
        free(temp);
    }
}
