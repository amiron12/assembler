
#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int is_directive(char *str)
{
    return (*str=='.')?TRUE:FALSE;
}
int is_immediate(char *str) {return (*str=='#')?TRUE:FALSE;}
int is_relative(char *str) {return (*str=='%')?TRUE:FALSE;}
int is_data(char *str) {return !strcmp(str, ".data")?TRUE:FALSE;}
int is_string(char *str) {return !strcmp(str, ".string")?TRUE:FALSE;}
int is_extern(char *str) {return !strcmp(str, ".extern")?TRUE:FALSE;}
int is_entry(char *str) {return !strcmp(str, ".entry")?TRUE:FALSE;}

int is_label(char *str)
{
    int len, i;
    len = strlen(str);
    if(str[len-1]!=':' || len>LABEL_LENGTH) return FALSE;
    if(!isalpha(*str)) return FALSE; /* first character is non alphabetical */
    for(i=1;i<len-1;i++)
        if(!isalnum(str[i])) return FALSE;
    return TRUE;
}

int is_instruction(char *str) 
{
    char *temp;
    int i = 0;
    while((temp = get_instruction_name(i++)) != NULL)
        if(!strcmp(str, temp))
            return TRUE;
    return FALSE;
}

int is_register(char *str)
{
    if(strlen(str)!=2) return FALSE;
    return(str[0]=='r' && (str[1] >= '0' && str[1] <= '7'))?TRUE:FALSE; /* TODO: change nums */
}

/*removing non alpha-numeric characters in the begining or end of the string*/
void clean_string(char **str)
{

    int len = strlen(*str);
    while (len > 0 && !isalnum((unsigned char)(*str)[len - 1])) {
        (*str)[len - 1] = '\0';
        len--;
    }

    
    while ((*str)[0] != '\0' && !isalnum((unsigned char)(*str)[0])) {
        (*str)++;
    }
}

op_mode get_mode(char *str)
{

    if(is_immediate(str)) return IMM;
    if(is_directive(str)) return DIR;
    if(is_relative(str)) return REL;
    if(is_register(str)) return REG;
    return 1; /* TODO: refine */
}

static int is_int(char *s)
{
    char *e;
    if (!s || !*s) return 0;
    strtol(s, &e, DEC);
    return *e == '\0';
}
   

int validate_data(char *args[])
{
    while (*args)
        if(!is_int(*args++)) return FALSE;
    return TRUE;
}

int validate_string(char *str)
{
    if(str[0] == '\"' && str[strlen(str)-1] == '\"')
        return TRUE;
    return FALSE;
}

line_type sentence_type(char *str)
{
    if(is_directive(str)) return directive;
    return instruction;
}

attribute dir_type(char *str)
{
    if(is_data(str) || is_string(str)) return data;
    if(is_extern(str)) return external;
    return entry;
}



