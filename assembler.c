
#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 /* TODO: move to header file: */

#define LABEL_LENGTH 31 /* 31 chars */

int DC; /* data counter: .data .string */
int IC; /* instruction counter: .text */
struct 
{
    char *name;
    int opcode;
    int funct;

} 
instructions[] = {
    {"mov", 0, 0},
    {"cmp", 1, 0},
    {"add", 2, 10},
    {"sub", 2, 11},
    {"lea", 4, 0},
    {"clr", 5, 10},
    {"not", 5, 11},
    {"inc", 5, 12},
    {"dec", 5, 13},
    {"jmp", 9, 10},
    {"bne", 9, 11},
    {"jsr", 9, 12},
    {"red", 12, 0},
    {"prn", 13, 0},
    {"rts", 14, 0},
    {"stop", 15, 0},
    {NULL, NULL, NULL}
};


struct
{
}
labels[] = {

};



int main(int argc, char *argv[])
{
    int i;
    FILE *fp;
    int c;
    char **files = malloc((argc) * sizeof(char*));
    
    if(!strcmp(argv[ZERO], "assembler")) /* TODO: do i suppose its legal */
    
    for(i=ONE;i<argc;i++)
    {
        strcpy(files[i], argv[i]);
        strcat(files[i], EXTENTION);
    }

    


    return 0;
}


