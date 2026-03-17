#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "output.h"
#include "symbols.h"
#include "const_tables.h"
#include "structs.h" 
#include "constants.h"



/* pointer validations */

void memory_check(void *ptr)
{
    if(ptr) return;
    printf("Fatal Error: Memory alocation failed\n");
    exit(1);
}

void file_check(FILE *fp) /* TODO: delete */
{
    if(fp) return;
    printf("Fatal Error: File opening failed\n");  
    exit(1);
}


void error(file_data *fs, char *str)
{
    printf("File: %s Line: %d - %s\n", fs->extended_name, fs->current_line, str);
    fprintf(stderr,"[ERROR] File: %s Line: %d - %s\n", fs->extended_name, fs->current_line, str);
    fs->error_flag = TRUE;
}


void extention(file_data *fs, char *ext)
{
    char fname[MAX_FNAME];
    strcpy(fname, (fs)->name);
    strcat(fname, ext);
    strncpy((fs)->extended_name, fname, MAX_FNAME);
}


int is_directive(char *str){return (*str=='.')?TRUE:FALSE;}
int is_data(char *str) {return !strcmp(str, ".data")?TRUE:FALSE;}
int is_entry(char *str) {return !strcmp(str, ".entry")?TRUE:FALSE;}
int is_extern(char *str) {return !strcmp(str, ".extern")?TRUE:FALSE;}
int is_string(char *str) {return !strcmp(str, ".string")?TRUE:FALSE;}
int is_immediate(char *str) {return (*str=='#')?TRUE:FALSE;}
int is_relative(char *str) {return (*str=='%')?TRUE:FALSE;}


/* general use function to check if a given string is a reserved word of the machine */
int reserved(char *str)
{
    if(is_instruction(str) || is_register(str) || is_data(str) || is_entry(str) || is_extern(str) || is_string(str)) return TRUE;
    return FALSE;
}

/* general use function to make sure that a symbol is defined by the correct rules */
void validate_symbol(char *str, file_data *fs)
{
    int i, len;
    len = strlen(str);
    
    if(str == NULL)
        error(fs, "Symbol not specified");

    if(str[len-1]==':') /* if its a symbol defenition in the start of a line */
    {
        clean_string(&str);
        len--;
    }

    if(len > MAX_SYMBOL_LENGTH)
        error(fs, "Symbol name is too long"); /* symbol too long */

    if (!isalpha(*str))
        error(fs, "Invalid symbol name"); /* first character is non alphabetical */
    
    if(is_macro_call(str, fs->macro_list))
        error(fs, "Symbol cannot be a macro name");
    
    if(symbol_exists(str, fs->symbol_list))
        error(fs, "Symbol already exists");
    
    if(reserved(str))
        error(fs, "Symbol cannot be a reserved word");

    for(i=0;i<len;i++)
        if(!isalnum(str[i]))
            error(fs, "Invalid symbol name"); /* symbol contains a non alpha-numeric character */
    
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
void clean_string(char **str) /* TODO: refine: need all the casting? */
{
    int len = strlen(*str);
    while (len > 0 && !isalnum((unsigned char)(*str)[len - 1])) 
    {
        (*str)[len - 1] = '\0';
        len--;
    }
    while ((*str)[0] != '\0' && !isalnum((unsigned char)(*str)[0])) 
        (*str)++;
}

int get_mode(char *str)
{
    if(is_immediate(str)) return IMM;
    if(is_relative(str)) return REL;
    if(is_register(str)) return REG;
    return DIR;
}

static int is_int(char *s)
{
    char *c;
    if (!s || !*s) return 0;
    strtol(s, &c, DEC);
    return *c == '\0';
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


/* function for aborting the proccess of a file, mainly is the same functionality
as free_data function, but is referenced when an error occurs */
void abort_file(file_data *f)
{
    fprintf(stderr,"[ERROR] Aborting file\n");
    delete_output_files(f->name); /* will delete if created */
    free_data(f);
}

/* function that wraps different memory freeing functions
all free functions are called, those that receive NULL pointers return immidietly */
void free_data(file_data *f)
{
    free_symbols(f->symbol_list);
    free_macros(f->macro_list);
    fclose(f->ptr);
    fprintf(stderr,"[INFO] Data free'd\n");
}

/* this function receives a pointer to a line of text, extracts the first word (ends with the first whitespace)
 and leaves the original pointer as the remaining of the line */
void get_next_word(char **line, char **word)
{
    *word = strtok(*line, " \t\n\r");
    *line = strtok(NULL, "");
}



   