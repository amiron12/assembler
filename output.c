#include "output.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "constants.h"
#include "structs.h"
#include "utils.h"
#include "machine_image.h"


void init_output_files(file_data *fs)
{
    FILE *f;
    char name[MAX_FILE_NAME];
    char *file_name = fs->name;

    strcpy(name, file_name);
    strcat(name, ENT);
    f = fopen(name, "w"); 
    if(!f)
        error(fs, "Error opening .ent file");
    fclose(f);

    strcpy(name, file_name);
    strcat(name, EXT);
    f = fopen(name, "w"); 
    if(!f)
        error(fs, "Error opening .ext file");
    fclose(f);
}

void init_file_data(file_data *fs, char *fname, char *ext, char *mode)
{
    strncpy(fs->name, fname, MAX_FILE_NAME);
    extention(fs, ext);
    fs->ptr = fopen(fs->extended_name, mode);
    fs->error_flag = FALSE;
    fs->current_line = ZERO;
    fs->macro_list = NULL;
    fs->symbol_list = NULL;
    if((fs->ptr)) return;
    error(fs, "Error opening file");
}

void create_obj_file(file_data *fs)
{
    FILE *ob_file;
    int i;
    char fname[MAX_FILE_NAME];
    char *file_name = fs->name;

    strcpy(fname, file_name);
    strcat(fname, OBJ);
    ob_file = fopen(fname, "w+");
    if(!ob_file)
    {
        error(fs, "Error opening .ob file");
        return;
    }

    IC = ICF;

    fprintf(ob_file,"%d %d",IC_INDEX, DCF);
    for(i=0;i<IC_INDEX;i++)
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

int append_entry(symbol *entry, file_data *fs)
{
    FILE *ent_file;
    char name[MAX_FILE_NAME];
    char *file_name = fs->name;
    strcpy(name, file_name);
    strcat(name, ENT);
    ent_file = fopen(name, "a");
    if(!ent_file)
    {
        error(fs, "Error opening .ent file");
        return FALSE;
    }
    fprintf(ent_file, "%s %04d\n", entry->name, entry->address);
    fclose(ent_file);
    return TRUE;
}

int append_external(symbol *external, file_data *fs)
{
    FILE *ext_file;
    char name[MAX_FILE_NAME];
    char *file_name = fs->name;
    strcpy(name, file_name);
    strcat(name, EXT);
    ext_file = fopen(name, "a");
    if(!ext_file)
    {
        error(fs, "Error opening .ext file");
        return FALSE;
    }
    fprintf(ext_file, "%s %04d\n", external->name, IC);
    fclose(ext_file);
    return TRUE;
}

void delete_output_files(char *file_name)
{
    delete_ent_file(file_name);
    delete_ext_file(file_name);
    delete_obj_file(file_name);
}

void delete_ent_file(char *file_name)
{
    char name[MAX_FILE_NAME];
    strcpy(name, file_name);
    strcat(name, ENT);
    remove(name);
}

void delete_ext_file(char *file_name)
{
    char name[MAX_FILE_NAME];
    strcpy(name, file_name);
    strcat(name, EXT);
    remove(name);
}

void delete_obj_file(char *file_name)
{
    char name[MAX_FILE_NAME];
    strcpy(name, file_name);
    strcat(name, OBJ);
    remove(name);
}
