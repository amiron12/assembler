
#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 /* TODO: move to header file: */

#define LABEL_LENGTH 31 /* 31 chars */

int DC; /* data counter: .data .string */
int IC; /* instruction counter: .text */




int main(int argc, char *argv[])
{



int i;
FILE *fpas, *fpam;
char fname[100], fname2[100];
/*
int index = 0;
inst instructions[17];
instructions[index].name = "mov";
instructions[index].opcode = 0;
instructions[index].funct = 0;
instructions[++index].name = "cmp";
instructions[index].opcode = 1;
instructions[index].funct = 0;
instructions[++index].name = "add";
instructions[index].opcode = 2;
instructions[index].funct = 10;
instructions[++index].name = "sub";
instructions[index].opcode = 2;
instructions[index].funct = 11;
instructions[++index].name = "lea";
instructions[index].opcode = 4;
instructions[index].funct = 0;
instructions[++index].name = "clr";
instructions[index].opcode = 5;
instructions[index].funct = 10;
instructions[++index].name = "not";
instructions[index].opcode = 5;
instructions[index].funct = 11;
instructions[++index].name = "inc";
instructions[index].opcode = 5;
instructions[index].funct = 12;
instructions[++index].name = "dec";
instructions[index].opcode = 5;
instructions[index].funct = 13;
instructions[++index].name = "jmp";
instructions[index].opcode = 9;
instructions[index].funct = 10;
instructions[++index].name = "bne";
instructions[index].opcode = 9;
instructions[index].funct = 11;
instructions[++index].name = "jsr";
instructions[index].opcode = 9;
instructions[index].funct = 12;
instructions[++index].name = "red";
instructions[index].opcode = 12;
instructions[index].funct = 0;
instructions[++index].name = "prn";
instructions[index].opcode = 13;
instructions[index].funct = 0;
instructions[++index].name = "rts";
instructions[index].opcode = 14;
instructions[index].funct = 0;
instructions[++index].name = "stop";
instructions[index].opcode = 15;
instructions[index].funct = 0;
instructions[++index].name = NULL;
instructions[index].opcode = 0;
instructions[index].funct = 0;
*/


    /* TODO: assembler command */

    

    for(i=1;i<argc;i++)
    {
        strcpy(fname, argv[i]);
        strcpy(fname2, argv[i]);
        strcat(fname, EXT_AS);
        strcat(fname2, EXT_AM);
        fpas = fopen(fname, "r");
        if(fpas == NULL) return 1;
        fpam = fopen(fname2, "w");
        if(fpam == NULL) return 1;
        expand_macros(fpas, fpam);
        break;
        if(fpam == NULL)
            continue;
        /*start_pass(fpam);*/
        fclose(fpas);
        fclose(fpam);
    }

    return 0;
}




