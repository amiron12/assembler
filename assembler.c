
#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void init_file_state(file_state *fs, char *fname, char *ext);
void create_files(char *file_name, symbol *symbol_table);
void create_ent(symbol *head, char *name);
void create_ext(symbol *head, char *name);
int main(int argc, char *argv[])
{
    int i;
    char *fname;    
    file_state am_file;
    file_state as_file;
    symbol *symbol_table;
    i = 0;
    
    while(++i<argc)
    {
        DC = ZERO;
        IC = MEM_START;
        fname = argv[i];

        init_file_state(&as_file, fname, AS);
        if(as_file.error_flag) continue;

        expand_macros(&as_file); /* pre-assembler stage */
        if(as_file.error_flag) continue;

        init_file_state(&am_file, fname, AM);
        if(am_file.error_flag) continue;
        
        symbol_table = first_pass(&am_file); /* starting first pass */
        if(am_file.error_flag) continue;

        /* TODO: create .ob, .ext, .ent */
        create_files(fname, symbol_table);
        fclose(am_file.ptr);
        fclose(as_file.ptr);
    }

    
    printf("finished\n"); 
    return 0;
}


void init_file_state(file_state *fs, char *fname, char *ext)
{
    strncpy(fs->name, fname, MAX_FNAME);
    extention(fs, ext);
    fs->ptr = fopen(fs->extended_name, "r");
    fs->error_flag = FALSE;
    fs->current_line = ZERO;
    if(!fs->ptr) 
    {
        fs->error_flag = TRUE;
        printf("File \"%s\" opening failed\n", fs->extended_name);
    }
}

void create_files(char *file_name, symbol *symbol_table)
{
    /* creating object file */
    FILE *ob_file;
    int i;
    char fname[MAX_FNAME];
    strcpy(fname, file_name);
    strcat(fname, OBJ);
    ob_file = fopen(fname, "w+");
    file_check(ob_file);
    IC = ICF;
    DC = DCF;

    fprintf(ob_file,"%d %d",ICINDEX, DCF);
    for(i=0;i<ICINDEX;i++)
    {
        short w;
        char c;
        w = code_image[i].word;
        c = code_image[i].type;
        fprintf(ob_file,"\n%04d %03X %c", i+MEM_START, w, c);
    }

    for(i=0;i<DCF;i++)
    {
        short w;
        char c;
        w = data_image[i].word;
        c = data_image[i].type;
        fprintf(ob_file,"\n%04d %03X %c", i+ICF, w, c);
    }
    fclose(ob_file);

    create_ent(symbol_table, file_name);
    create_ext(symbol_table, file_name);
}

void create_ent(symbol *head, char *file_name)
{
    char name[MAX_FNAME];
    FILE *ent;
    int flag;
    symbol *temp = head;
    flag = FALSE;
    strcpy(name, file_name);
    while(temp != NULL && !flag)
    {
        if(temp->atr == entry)
            flag = TRUE;
        temp = temp->next;
    }
    if(!flag)
        return;
    temp = head;

    strcat(name, ENT);
    ent = fopen(name, "w+");
    file_check(ent);

    while (temp != NULL)
    {
        if(temp->atr == entry)
        {
            fprintf(ent, "%s %04d\n", temp->name, temp->address);
        }
        temp = temp->next;
    }     
}

void create_ext(symbol *head, char *file_name)  /* TODO: fix */
{
    char name[MAX_FNAME];
    FILE *ext;
    int flag;
    symbol *temp = head;
    flag = FALSE;
    strcpy(name, file_name);
    while(temp != NULL && !flag)
    {
        if(temp->atr == external)
            flag = TRUE;
        temp = temp->next;
    }
    if(!flag)
        return;
    temp = head;

    strcat(name, EXT);
    ext = fopen(name, "w+");
    file_check(ext);

    while (temp != NULL)
    {
        if(temp->atr == external)
        {
            fprintf(ext, "%s %04d\n", temp->name, temp->address);
        }
        temp = temp->next;
    }     
}
