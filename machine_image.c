
#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MASK 0xFFF
#define ABSOLUTE 'A'
#define RELATIVE 'R'
#define EXTERNAL 'E'
#define IMM 0
#define DIR 1
#define REL 2
#define REG 3

 machine_word data_image[MEMORY];
 machine_word code_image[MEMORY];



void encode_instruction(char *str, int src, int dest)
{
    int opcode, funct;
    opcode = get_instruction_opcode(str);
    funct = get_instruction_funct(str);
    code_image[IC].word = (((opcode<<8) | (funct<<4) | (src<<2) | (dest)) & MASK); /* TODO: replace numbers or formula overall */
    code_image[IC].type = 'A';
    IC++;
}


void encode_operand(char *str)
{

    int num;
    int val = 0;
    switch (get_mode(str))
    {
    case IMM:
        clean_string(&str);
        num = atoi(str);
        code_image[IC].word = (num & MASK);
        code_image[IC].type = ABSOLUTE;
        break;

    case REG:
        clean_string(&str);
        num = atoi(str);
        val = (1<<num);
        code_image[IC].word = (val & MASK);
        code_image[IC].type = ABSOLUTE;
        break;

     case REL|| DIR:
        code_image[IC].word = val;
        code_image[IC].type = '?';
        break;
    }
    IC++;
}


void encode_data(char *args[])
{
    while(*args != NULL)
        data_image[DC++].word = (atoi((*args)++) & MASK);
}

void encode_string(char *str)
{
    clean_string(&str);
    while(*str != '\0')
        data_image[DC++].word = (*str++ & MASK);
    data_image[DC++].word = ZERO;
}



