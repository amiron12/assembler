#ifndef CONST_TABLES_H
#define CONST_TABLES_H
#define M0 1
#define M1 2
#define M2 4
#define M3 8


typedef struct inst
{
    char *name;
    int opcode;
    int funct;
    int operands;
    unsigned int src_modes;
    unsigned int dest_modes;
} inst;



/* Retrieves the name of an instruction by its index in the instructions table */
char *get_instruction_name(int index);

/* Retrieves the expected number of operands for a given instruction name */
int get_instruction_operands(char *name);

/* Retrieves the opcode for a given instruction name */
int get_instruction_opcode(char *name);

/* Retrieves the funct value for a given instruction name */
int get_instruction_funct(char *name);

/* Validates if the given addressing mode is allowed as a destination operand for the instruction */
int validate_dest_mode(char *name, int mode);

/* Validates if the given addressing mode is allowed as a source operand for the instruction */
int validate_src_mode(char *name, int mode);


#endif
