/*
 * machine_image.c
 * This file contains functions that translate instructions, data, and strings 
 * into machine code words. It manages the encoding of operands based on their 
 * addressing modes and populates the code and data images that form the 
 * final executable representation.
 */

#include "machine_image.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "const_tables.h"
#include "constants.h"
#include "structs.h"
#include "utils.h"
#include "string_utils.h"

/**
 * This function receives a string representing an operand and a file_data pointer.
 * It determines the addressing mode of the operand, parses its value, and
 * encodes the resulting machine word into the code image array, updating the type
 * and instruction counter (IC).
 */
static void encode_operand(char *str, file_data *fs, char* file_name)
{

    int num;
    int val = 0;
    if(str == NULL) return;
    switch (get_mode(str))
    {
        case IMM:
            clean_string(&str, '#');
            if(!validate_number(str))
            {
                error(fs, file_name, "Invalid immediate value");
                return;
            }
            num = atoi(str);
            fs->code_image[IC_INDEX(fs)].word = (num & MASK);
            fs->code_image[IC_INDEX(fs)].type = ABSOLUTE;
            break;

        case REG:
        {
            char *c;
            c = &str[1];
            num = atoi(c);
            val = (1<<num);
            fs->code_image[IC_INDEX(fs)].word = (val & MASK);
            fs->code_image[IC_INDEX(fs)].type = ABSOLUTE;
            break;
        }

        case DIR:
            fs->code_image[IC_INDEX(fs)].word = val;
            fs->code_image[IC_INDEX(fs)].type = UNKNOWN;
            break;
        

        case REL:
            fs->code_image[IC_INDEX(fs)].word = val;
            fs->code_image[IC_INDEX(fs)].type = ABSOLUTE;
            break;
        }
    fs->IC++;
}

/**
 * This function receives an instruction name, its two optional arguments, and a
 * file_data pointer. It validates the addressing modes for the operands, encodes
 * the main instruction word with the opcode, funct, and operand modes, and then
 * calls encode_operand to encode any subsequent operand words.
 */
void encode_instruction(char *str, char *arg1, char *arg2, file_data *fs, char* file_name)
{
    int opcode, funct, val;
    int src_mode, dest_mode;
    int mode_err = FALSE;
    src_mode = dest_mode = opcode = funct = val = ZERO;

    if(arg1 != NULL && arg2 == NULL)
    {
        dest_mode = get_mode(arg1);
        if(!validate_dest_mode(str, dest_mode))
            mode_err = TRUE;
    }
    else if(arg2 != NULL)
    {
        src_mode = get_mode(arg1);
        dest_mode = get_mode(arg2);
        if(!validate_dest_mode(str, dest_mode) || !validate_src_mode(str, src_mode))
            mode_err = TRUE;
    }

    if(mode_err)
    {
        error(fs, file_name, "Invalid addressing mode");
        return;
    }
    
    opcode = get_instruction_opcode(str);
    funct = get_instruction_funct(str);
    val = ((opcode<<OP_SHIFT) | (funct<<FUNCT_SHIFT) | (src_mode<<SRC_SHIFT) | (dest_mode)); 
    fs->code_image[IC_INDEX(fs)].word = val & MASK;
    fs->code_image[IC_INDEX(fs)].type = ABSOLUTE;
    fs->IC++;

    encode_operand(arg1,fs, file_name);
    encode_operand(arg2,fs, file_name);

} 


/**
 * This function receives an array of string arguments representing integers
 * for the .data directive. It parses each integer, masks it to fit within a machine
 * word, and encodes it into the data image array, updating the data counter (DC).
 */
void encode_data(char *args[], file_data* file)
{

    int i = 0; 
    while(args[i] != NULL)
        {
            file->data_image[file->DC].word = (strtol(args[i++], NULL, DEC) & MASK);
            file->data_image[file->DC++].type = ABSOLUTE;
        }
}

/**
 * This function receives a string literal for the .string directive.
 * It strips the quotation marks, iterates through each character, and encodes
 * the ASCII values into the data image array, ending with a null terminator.
 */
void encode_string(char *str, file_data* file)
{
    clean_string(&str, '"');
    while(*str != '\0')
    {
        file->data_image[file->DC].word = (*str++ & MASK);
        file->data_image[file->DC++].type = ABSOLUTE;
    }
    file->data_image[file->DC].word = ZERO;
    file->data_image[file->DC++].type = ABSOLUTE;
}



