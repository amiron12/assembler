#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* TODO: delete file */

/* TODO: delete function */
void turn_on(unsigned char *c, int num)
{
    int byte; /* Byte index in the array */
    int shif_value; /* Bit position within the byte */
    byte = num/BYTE;
    shif_value = (BYTE-ONE) - (num%BYTE);
    c[byte] |= (ONE<<shif_value);
}

/* TODO: move to another file */
void dec_to_bin(char str[], machine_word *word) /* Function that receives a char array that simulates a number and prints it as binary */
{
    int size = (sizeof(unsigned int)*8)-1; /* TODO: remove numbers */
    int flag = 0; /* flag to check the first appearance of '1' */
    unsigned int num = atoi(str);
    while(size>=0)
    {
        int tmp = 1;
        tmp<<=size--;
        tmp&=num;
        if(tmp!=0)
            flag=1; /* start printing */
        if(flag) 
            printf("%d",(tmp==0)?0:1);
    }
}

