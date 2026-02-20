
#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int is_directive(char *str) {(*str=='.')?TRUE:FALSE;}
int is_immediate(char *str) {return (*str=='#')?TRUE:FALSE;}
int is_relative_label(char *str) {return (*str=='%')?TRUE:FALSE;}
int is_data(char *str) {return !strcmp(str, ".data")?TRUE:FALSE;}
int is_string(char *str) {return !strcmp(str, ".string")?TRUE:FALSE;}
int is_extern(char *str) {return !strcmp(str, ".extern")?TRUE:FALSE;}
int is_entry(char *str) {return !strcmp(str, ".entry")?TRUE:FALSE;}

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

int is_instruction(char *str) /* TODO: move to func utils and use for reserve check */
{
    char *temp;
    int i = 0;
    while((temp = get_instruction_name(i++)) != NULL)
        if(!strcmp(str, temp))
            return FALSE;
    return TRUE;
}


int is_register(char *str)
{
    if(strlen(str)!=2) return FALSE;
    return(str[0]=='r' && (str[1] >= '0' && str[1] <= '7'))?TRUE:FALSE;
}

char* clean_label(char *str)
{
    if(str==NULL) return NULL;
    int len = strlen(str);
    str[--len] = '\0';
    return str;
}

int string_check(char *str)
{

}

int data_check(char *str)
{
    
}