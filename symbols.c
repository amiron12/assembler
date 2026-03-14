#include "symbols.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "constants.h"
#include "utils.h"

/* This functions receives a symbol list and all information for a new symbol
 it pushes a new symbol node with this information, and sets it as the head of the list */
void add_symbol(char *symbol_name, symbol **head, int address, attribute attr)
{
    symbol *new_node = (symbol *)malloc(sizeof(symbol));
    memory_check(new_node);
    strncpy(new_node->name, symbol_name, SYMBOL_LENGTH);
    new_node->address = address;
    (new_node->attr) = (1<<attr); /* turns on the required bit */
    new_node->next = *head;
    *head = new_node;
}

/* This functions receives a string and a symbol list
 it checks if there is a symbol with the string as its name
 returns the symbol if it exists, otherwise returns NULL */
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

/* This functions receives a string and a symbol list
 it checks if there is a symbol with the string as its name
 returns a non-zero value if it does not exists, otherwise returns zero */
int symbol_exists(char *name, symbol *head)
{
   return get_symbol(name, head) != NULL;
}

/* This function receives a symbol and an attribute and checks if the symbol holds this attribute
 if the attribute bit flag is turned on, it returns a non zero value, otherwise returns zero */
int is_attribute(symbol *sym, attribute attr)
{
    int val = 0;
    val = (1<<attr);
    return (sym->attr) & val;
}

/* This function gets a symbol node, and an attribute
 it turns on the bit that is identified with this attribute without deleting other attributes*/
void set_attribute(symbol *sym, attribute attr)
{
    (sym->attr) |= (1<<attr);
}

/* This function goes through the whole symbol list and frees its memory */
void free_symbols(symbol *head)
{
    if(head != NULL)
    {
        void *temp = head;
        head = head->next;
        free(temp);
    }
    fprintf(stderr,"[INFO] Symbol list free'd\n");
}
