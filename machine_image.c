
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
#define ICINDEX (IC)-100

 machine_word data_image[MEMORY];
 machine_word code_image[MEMORY];



void encode_instruction(char *str, int src, int dest)
{
    int opcode, funct, val;
    opcode = get_instruction_opcode(str);
    funct = get_instruction_funct(str);
    val = ((opcode<<8) | (funct<<4) | (src<<2) | (dest));
    code_image[ICINDEX].word = val & MASK;
    code_image[ICINDEX].type = ABSOLUTE;
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
        code_image[ICINDEX].word = (num & MASK);
        code_image[ICINDEX].type = ABSOLUTE;
        break;

    case REG:
        clean_string(&str);
        num = atoi(str);
        val = (1<<num);
        code_image[ICINDEX].word = (val & MASK);
        code_image[ICINDEX].type = ABSOLUTE;
        break;

     case REL|| DIR:
        code_image[ICINDEX].word = val;
        code_image[ICINDEX].type = '?';
        break;
    }
    IC++;
}


void encode_data(char *args[])
{
    int i = 0;
    while(args[i] != NULL)
        data_image[DC++].word = (atoi(args[i++]) & MASK);
}

void encode_string(char *str)
{
    clean_string(&str);
    while(*str != '\0')
        data_image[DC++].word = (*str++ & MASK);
    data_image[DC++].word = ZERO;
}



