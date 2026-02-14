
#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 /* TODO: move to header file: */

#define LABEL_LENGTH 31 /* 31 chars */

int DC; /* data counter: .data .string */
int IC; /* instruction counter: .text */

inst instructions[] = {
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
    /* TODO: assembler command */
    int i;
    FILE *fpas, *fpam;
    for(i=0;i<argc;i++)
    {
        


    }


    return 0;
}




