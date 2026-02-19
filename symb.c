#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int label_exist(char *name, symbol *head)
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

void add_symbol(char *symbol_name, symbol **curr, int address, attribute atr)
{
    symbol *temp = (symbol *)malloc(sizeof(symbol));
    memory_check(temp);
    strncpy(temp->name, symbol_name, LABEL_LENGTH);
    temp->next = NULL;
    temp->address = address;
    temp->atr = atr;
    if(*curr == NULL)
        *curr = temp;
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

void print_symbols(symbol *head)
{
    symbol *temp;
    temp = head;
    while(temp != NULL)
    {
        printf("%s\n", temp->name);
        temp = temp->next;
    }
}

