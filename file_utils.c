#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "constants.h"
#include "symbol_table.h"
#include "file_utils.h"
#include "utils.h"
#include "machine_image.h"







void create_obj_file(char *file_name, symbol *symbol_table)
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
}

void create_ent_file(symbol *head, char *file_name)
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


void create_ext_file(symbol *head, char *file_name)  /* TODO: fix */
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