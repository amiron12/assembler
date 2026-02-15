#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int symbol_check(char *token);

void start(FILE *fp)
{
    char line[MAX_LINE_LENGTH];
    char *token;
    symbol *head, *curr;
    head = NULL;
    curr = NULL;
    while(fgets(line, MAX_LINE_LENGTH, fp) != NULL)
    {
        token = strtok(line, " \t\n\r"); /* Get first word */
        if(token==NULL || !symbol_check(token)) continue; /* non symbol */
        token = strtok(token,":"); /* remove ':' */
        add_symbol(token, &curr);
        if(!head) head = curr; /* when head is still null */
    }

    print_symbols(head);
}






int symbol_check(char *token)
{
    int len, i;
    if(token==NULL) return FALSE;
    len = strlen(token);
    if(token[len-1]!=':' || len>LABEL_LENGTH) return FALSE;
    if(!isalpha(*token)) return FALSE; /* first character is non alphabetical */
    for(i=1;i<len-1;i++)
        if(!isalnum(token[i])) return FALSE;
    token = strtok(token,":");
    if(is_reserved(token)) return FALSE;
    return OK;
}

