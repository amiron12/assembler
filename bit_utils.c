#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
 * Function: turn_on
 * Purpose: Sets a specific bit to 1 in the byte array.
 * Input: c - Pointer to the byte array (set).
 *        num - The index of the bit to turn on (0-127).
 * Logic: Calculates byte index and bit offset, then ORs with shifted 1.
 * Assumptions: num is within valid range [0, 127].
 */
void turn_on(unsigned char *c, int num)
{
    int byte; /* Byte index in the array */
    int shif_value; /* Bit position within the byte */
    byte = num/BYTE;
    shif_value = (BYTE-ONE) - (num%BYTE);
    c[byte] |= (ONE<<shif_value);
}


void dec_to_bin(char str[]) /* Function that receives a char array that simulates a number and prints it as binary */
{
    int size = (sizeof(unsigned int)*8)-1;
    int flag = 0; /* flag to check the first appearance of '1' */
    unsigned int num = atoi(str);

    printf("Binary:\t");
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