/*
 * const_tables.c
 * This file contains constant tables and functions to retrieve information 
 * about the assembly language instructions. It defines the instruction 
 * set, including their names, opcodes, funct values, number of operands, 
 * and valid addressing modes for source and destination operands.
 */

#include "const_tables.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "structs.h"
#include "utils.h"

/**
 * This function receives an instruction name and returns
 * the instruction from the instructions table
 * or NULL if it is not found.
 */
inst* get_instruction(char* str)
{
    /* name, opmode, funct, number of ops, src modes, dest modes */
    static inst instructions[] = {
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
        {NULL, 0, 0, 0, 0, 0} };
    int i = 0;
    while (instructions[i].name != NULL)
    {
        if (!strcmp(str, instructions[i].name))
            return &instructions[i];
        i++;
    }
    return NULL;
}

/**
 * This function receives an instruction name and returns its corresponding
 * opcode by searching the instructions table.
 */
int get_instruction_opcode(char *name)
{
    inst* i = get_instruction(name);
    return i->opcode;
}

/**
 * This function receives an instruction name and returns its corresponding
 * funct value by searching the instructions table.
 */
int get_instruction_funct(char *name)
{
    inst* i = get_instruction(name);
    return i->funct;
}

/**
 * This function receives an instruction name and returns the number of
 * operands it expects by searching the instructions table.
 */
int get_instruction_operands(char *name)
{
    inst* i = get_instruction(name);
    return i->operands;
}

/**
 * This function receives an instruction name and an addressing mode,
 * and validates whether the mode is legal for the instruction's destination operand.
 * Returns non-zero if valid, zero otherwise.
 */
int validate_dest_mode(char *name, int mode) 
{
    inst* i = get_instruction(name);
    unsigned int mask = i->dest_modes;
    return (mask & (1 << mode)) != ZERO;
}
/**
 * This function receives an instruction name and an addressing mode,
 * and validates whether the mode is legal for the instruction's source operand.
 * Returns non-zero if valid, zero otherwise.
 */
int validate_src_mode(char *name, int mode) 
{
    inst* i = get_instruction(name);
    unsigned int mask = i->src_modes;
    return (mask & (1 << mode)) != 0;
}