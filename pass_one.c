#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int is_label(char *str);

void start(FILE *fp)
{
    char line[MAX_LINE_LENGTH];
    char *args[MAX_ARG_LENGTH];
    symbol *head, *curr;
    head = NULL;
    curr = NULL; 
    while(fgets(line, MAX_LINE_LENGTH, fp) != NULL)
    {
        int argc;
        char *label = NULL;
        argc = tokenize(line, args); /* saves all words in args, removing leading and trailing whitespaces, and trailing commas*/
        if(!argc || **args==';') continue;
        if(is_label(*args))
        {
            int len = strlen(*args);
            strcpy(label, *args);
            label[--len] = '\0';
            if(not_reserved(label)) 
                add_symbol(label, &curr);
        }

        
        

    }


}

static int word_type(char *str)
{
    if(str==NULL) return NULL; /* return 0 */

}

static int clean_word(char *str)
{

}

static int is_label(char *str)
{
    int len, i;
    len = strlen(str);
    if(str[len-1]!=':' || len>LABEL_LENGTH) return FALSE;
    if(!isalpha(*str)) return FALSE; /* first character is non alphabetical */
    for(i=1;i<len-1;i++)
        if(!isalnum(str[i])) return FALSE;
    return TRUE;
}

static int is_directive(char *str)
{
    (str[0]=='.')?TRUE:FALSE;
}

static int is_register(char *str)
{
   return (!strcmp(str, "r0") || !strcmp(str, "r1")
    || !strcmp(str, "r2") || !strcmp(str, "r3") || !strcmp(str, "r4")
    || !strcmp(str, "r5") || !strcmp(str, "r6") || !strcmp(str, "r7"))?TRUE:FALSE;
}

static int is_immediate(char *str)
{
    return (str[0]=='#')?TRUE:FALSE;
}



