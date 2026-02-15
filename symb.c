#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void add_symbol(char *symbol_name, symbol **curr)
{
    symbol *temp = (symbol *)malloc(sizeof(symbol));
    memory_check(temp);
    strncpy(temp->name, symbol_name, LABEL_LENGTH);
    temp->next = NULL;
    temp->address = IC++;
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