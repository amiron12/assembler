#include "symbol_table.h"
#include "constants.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int label_exist(char *name, symbol *head) /* TODO: change to symbol */
{
    symbol *temp;
    temp = head;
    while(temp != NULL)
    {
        if(!strcmp(temp->name, name))
            return TRUE;
        temp = temp->next;
    }
    /* TODO: error */
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
    /* TODO: error */
    return NULL;
}

void add_symbol(char *symbol_name, symbol **curr, symbol **head, int address, attribute atr)
{
    symbol *temp = (symbol *)malloc(sizeof(symbol));
    memory_check(temp);
    strncpy(temp->name, symbol_name, LABEL_LENGTH);
    temp->next = NULL;
    temp->address = address;
    temp->atr = atr;
    if(*head == NULL)
        *head = temp;
    else
        (*curr)->next = temp;
    *curr = temp;
}

void free_symbols(symbol *head)
{
    if(head != NULL)
    {
        void *temp = head;
        head = head->next;
        free(temp);
    }
}



/* TODO: make the names consistant - label/symbol */