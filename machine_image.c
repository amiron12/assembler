
#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MASK 0xFFF

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
    char c;
    int num;
    int mode = (get_mode(str));
    clean_string(&str);
    num = atoi(str);

    c = (mode==0 || mode==3)? 'A': '0';
    code_image[IC].word = (num & MASK);
    code_image[IC].type = c;
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



