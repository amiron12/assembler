#include "const_tables.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "structs.h"
#include "utils.h"

/* name, opmode, funct, number of ops, src modes, dest modes */
static const inst instructions[] = {
    {"mov", 0, 0, 2, M0 | M1 | M3, M1 | M3},
    {"cmp", 1, 0, 2, M0 | M1 | M3, M0 | M1 | M3},
    {"add", 2, 10, 2, M0 | M1 | M3, M1 | M3},
    {"sub", 2, 11, 2, M0 | M1 | M3, M1 | M3},
    {"lea", 4, 0, 2, M1, M1 | M3},
    {"clr", 5, 10, 1, 0, M1 | M3},
    {"not", 5, 11, 1, 0, M1 | M3},
    {"inc", 5, 12, 1, 0, M1 | M3},
    {"dec", 5, 13, 1, 0, M1 | M3},
    {"jmp", 9, 10, 1, 0, M1 | M2},
    {"bne", 9, 11, 1, 0, M1 | M2},
    {"jsr", 9, 12, 1, 0, M1 | M2},
    {"red", 12, 0, 1, 0, M1 | M3},
    {"prn", 13, 0, 1, 0, M0 | M1 | M3},
    {"rts", 14, 0, 0, 0, 0},
    {"stop", 15, 0, 0, 0, 0},
    {NULL, 0, 0, 0, 0, 0}};

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
    return instructions[index].operands;
}

int is_dest_allowed(char *name, int mode) 
{
    unsigned int mask = instructions[get_index(name)].dest_modes;
    return (mask & (1 << mode)) != ZERO;
}
int is_src_allowed(char *name, int mode) 
{
    unsigned int mask = instructions[get_index(name)].src_modes;
    return (mask & (1 << mode)) != 0;
}