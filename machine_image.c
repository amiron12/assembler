#include "machine_image.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "const_tables.h"
#include "constants.h"
#include "structs.h"
#include "utils.h"

static void encode_operand(char *str, file_data *fs)
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
                error(fs, "Invalid immediate value");
                return;
            }
            num = atoi(str);
            code_image[IC_INDEX].word = (num & MASK);
            code_image[IC_INDEX].type = ABSOLUTE;
            break;

        case REG:
        {
            char *c;
            c = &str[1];
            num = atoi(c);
            val = (1<<num);
            code_image[IC_INDEX].word = (val & MASK);
            code_image[IC_INDEX].type = ABSOLUTE;
            break;
        }

        case DIR:
            code_image[IC_INDEX].word = val;
            code_image[IC_INDEX].type = UNKNOWN;
            break;
        

        case REL:
            code_image[IC_INDEX].word = val;
            code_image[IC_INDEX].type = ABSOLUTE;
            break;
        }
    IC++;
}

void encode_instruction(char *str, char *arg1, char *arg2, file_data *fs)
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
        error(fs, "Invalid addressing mode");
        return;
    }
    
    opcode = get_instruction_opcode(str);
    funct = get_instruction_funct(str);
    val = ((opcode<<OP_SHIFT) | (funct<<FUNCT_SHIFT) | (src_mode<<SRC_SHIFT) | (dest_mode)); 
    code_image[IC_INDEX].word = val & MASK;
    code_image[IC_INDEX].type = ABSOLUTE;
    IC++;

    encode_operand(arg1,fs);
    encode_operand(arg2,fs);

} 


void encode_data(char *args[])
{

    int i = 0; 
    while(args[i] != NULL)
        {
            data_image[DC].word = (strtol(args[i++], NULL, DEC) & MASK);
            data_image[DC++].type = ABSOLUTE;
        }
}

void encode_string(char *str)
{
    clean_string(&str, '"');
    while(*str != '\0')
    {
        data_image[DC].word = (*str++ & MASK);
        data_image[DC++].type = ABSOLUTE;
    }
    data_image[DC].word = ZERO;
    data_image[DC++].type = ABSOLUTE;
}



