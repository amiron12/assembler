/*
 * output.c
 * This file handles the generation of the output files (.ob, .ent, .ext) 
 * after the assembler passes are complete. It creates the object file 
 * representing the machine image and writes symbol information to the 
 * entry and external files as needed.
 */

#include "output.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "constants.h"
#include "structs.h"
#include "utils.h"
#include "machine_image.h"

/**
 * This function is used at the beginning of the second pass to open 
 * the output files (.ent, .ext) in write mode to clear their contents.
 * Returns ZERO on success, or ERR if opening fails.
 */
int init_output_files(file_data *fs, char* file_name)
{
    FILE *f;
    char name[MAX_FILE_NAME];

    strcpy(name, file_name);
    strcat(name, ENT);
    f = fopen(name, "w"); 
    if(!f)
        error(fs, name, "Error opening .ent file");
    fclose(f);

    strcpy(name, file_name);
    strcat(name, EXT);
    f = fopen(name, "w"); 
    if(!f)
        error(fs, name, "Error opening .ext file");
    fclose(f);

    if(!(fs->error_flag))
        return ZERO;

    delete_output_files(file_name);
    free_data(fs);

    return ERR;
}

/**
 * This function generates the final object (.ob) file. It writes the 
 * instruction and data counters, followed by the memory image 
 * (instructions and data) formatted as required.
 */
void create_obj_file(file_data *fs, char* file_name)
{
    FILE *ob_file;
    int i;
    char fname[MAX_FILE_NAME];

    strcpy(fname, file_name);
    strcat(fname, OBJ);
    ob_file = fopen(fname, "w+");
    if(!ob_file)
    {
        error(fs, file_name, "Error opening .ob file");
        return;
    }

    fs->IC = fs->ICF;

    fprintf(ob_file,"%d %d",IC_INDEX(fs), fs->DCF);
    for(i=0;i<IC_INDEX(fs);i++)
    {
        short w;
        char c;
        w = fs->code_image[i].word;
        c = fs->code_image[i].type;
        fprintf(ob_file,"\n%04d %03X %c", i+MEM_START, w, c);
    }

    for(i=0;i< fs->DCF;i++)
    {
        short w;
        char c;
        w = fs->data_image[i].word;
        c = fs->data_image[i].type;
        fprintf(ob_file,"\n%04d %03X %c", i+fs->ICF, w, c);
    }
    fclose(ob_file);
}

/**
 * This function appends an entry symbol and its resolved address to 
 * the entries (.ent) output file. Returns TRUE on success, FALSE on failure.
 */
int append_entry(symbol *entry, file_data *fs, char* fname)
{
    FILE *ent_file;
    char name[MAX_FILE_NAME];
    char *file_name = fname;
    strcpy(name, file_name);
    strcat(name, ENT);
    ent_file = fopen(name, "a");
    if(!ent_file)
    {
        error(fs, name, "Error opening .ent file");
        return FALSE;
    }
    fprintf(ent_file, "%s %04d\n", entry->name, entry->address);
    fclose(ent_file);
    return TRUE;
}

/**
 * This function appends an external symbol and the address where it is 
 * used to the externals (.ext) output file. Returns TRUE on success, 
 * FALSE on failure.
 */
int append_external(symbol *external, file_data *fs, char* fname)
{
    FILE *ext_file;
    char name[MAX_FILE_NAME];
    char *file_name = fname;
    strcpy(name, file_name);
    strcat(name, EXT);
    ext_file = fopen(name, "a");
    if(!ext_file)
    {
        error(fs, file_name, "Error opening .ext file");
        return FALSE;
    }
    fprintf(ext_file, "%s %04d\n", external->name, fs->IC);
    fclose(ext_file);
    return TRUE;
}

/**
 * This function deletes all generated output files (.ent, .ext, .ob) 
 * for a given base file name, typically used when an error is encountered.
 */
void delete_output_files(char *file_name)
{
    delete_ent_file(file_name);
    delete_ext_file(file_name);
    delete_obj_file(file_name);
}

/**
 * This function deletes the entries (.ent) file for a given base file name.
 */
void delete_ent_file(char *file_name)
{
    char name[MAX_FILE_NAME];
    strcpy(name, file_name);
    strcat(name, ENT);
    remove(name);
}

/**
 * This function deletes the externals (.ext) file for a given base file name.
 */
void delete_ext_file(char *file_name)
{
    char name[MAX_FILE_NAME];
    strcpy(name, file_name);
    strcat(name, EXT);
    remove(name);
}

/**
 * This function deletes the object (.ob) file for a given base file name.
 */
void delete_obj_file(char *file_name)
{
    char name[MAX_FILE_NAME];
    strcpy(name, file_name);
    strcat(name, OBJ);
    remove(name);
}
