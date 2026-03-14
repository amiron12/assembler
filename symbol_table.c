#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "constants.h"
#include "utils.h"

int symbol_exists(char *name, symbol *head)
{
    symbol *temp;
    temp = head;
    while(temp != NULL)
    {
        if(!strcmp(temp->name, name))
            return TRUE;
        temp = temp->next;
    }
    return FALSE;
}

symbol* get_symbol(char *name, symbol *head)
{
    symbol *temp;
    temp = head;
    while(temp != NULL)
    {
        if(!strcmp(temp->name, name))
            return temp;
        temp = temp->next;
    }
    return NULL;
}

void add_symbol(char *symbol_name, symbol **curr, symbol **head, int address, attribute attr)
{
    symbol *temp = (symbol *)malloc(sizeof(symbol));
    memory_check(temp);
    strncpy(temp->name, symbol_name, SYMBOL_LENGTH);
    temp->next = NULL;
    temp->address = address;
    (temp->attr) = (1<<attr); /* turns on the bit */

    if(*head == NULL)
        *head = temp;
    else
        (*curr)->next = temp;
    *curr = temp;
}

/* This function receives a symbol and an attribute and checks if the symbol holds this attribute
 if the attribute bit flag is turned on, it returns a non zero value, otherwise returns zero */
int is_attribute(symbol *sym, attribute attr)
{
    int val = 0;
    val = (1<<attr);
    return (sym->attr) & val;
}

void set_attribute(symbol *sym, attribute attr)
{
    (sym->attr) |= (1<<attr);
}

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
