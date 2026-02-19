#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct reg
{   
    int *ptr;
    char *name;
} reg;



typedef struct 
{
    char *name;
    int opcode;
    int funct;
    int operands;
    
} inst;

    inst instructions[] = {
    {"mov", 0, 0, 2},
    {"cmp", 1, 0, 2},
    {"add", 2, 10, 2},
    {"sub", 2, 11, 2},
    {"lea", 4, 0, 2},
    {"clr", 5, 10, 1},
    {"not", 5, 11, 1},
    {"inc", 5, 12, 1},
    {"dec", 5, 13, 1},
    {"jmp", 9, 10, 1},
    {"bne", 9, 11, 1},
    {"jsr", 9, 12, 1},
    {"red", 12, 0, 1},
    {"prn", 13, 0, 1},
    {"rts", 14, 0, 0},
    {"stop", 15, 0, 0},
    {NULL, 0, 0, 0}
};

char *get_instruction_name(int index)
{
    return instructions[index].name;
}

int get_instruction_opcode(char *name)
{
    int index = get_index(name);
    return instructions[index].opcode;
}

int get_instruction_funct(char *name)
{
    int index = get_index(name);
    return instructions[index].funct;
}

int get_instruction_operands(char *name)
{
    int index = get_index(name);
    return instructions[index].operands+1;
}


static int get_index(char *str)
{
    int i = 0;
    if(!is_instruction(str)) exit(1); /* TODO: error */
    while(instructions[i].name != NULL)
       {
        if(!strcmp(str, instructions[i].name))
            break;
        i++;
       }
    return i;
}