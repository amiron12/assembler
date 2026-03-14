#include "machine_image.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "const_tables.h"
#include "constants.h"
#include "structs.h"
#include "utils.h"

void encode_instruction(char *str, int src, int dest)
{
    int opcode, funct, val;
    opcode = get_instruction_opcode(str);
    funct = get_instruction_funct(str);
    val = ((opcode<<OP_SHIFT) | (funct<<FUNCT_SHIFT) | (src<<SRC_SHIFT) | (dest)); 
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
        {
            char *c;
            c = &str[1];
            num = atoi(c);
            val = (1<<num);
            code_image[ICINDEX].word = (val & MASK);
            code_image[ICINDEX].type = ABSOLUTE;
            break;
        }

        case DIR:
            code_image[ICINDEX].word = val;
            code_image[ICINDEX].type = UNKNOWN;
            break;
        

        case REL:
            code_image[ICINDEX].word = val;
            code_image[ICINDEX].type = ABSOLUTE;
            break;
        }
    IC++;
}


void encode_data(char *args[])
{
    int i = 0; 
    while(args[i] != NULL)
        {
            data_image[DC].word = (atoi(args[i++]) & MASK);
            data_image[DC++].type = ABSOLUTE;
        }
}

void encode_string(char *str)
{
    clean_string(&str);
    while(*str != '\0')
    {
        data_image[DC].word = (*str++ & MASK);
        data_image[DC++].type = ABSOLUTE;
    }
    data_image[DC].word = ZERO;
    data_image[DC++].type = ABSOLUTE;
}



