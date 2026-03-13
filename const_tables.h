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



char *get_instruction_name(int index);
int get_instruction_operands(char *name);
int get_instruction_opcode(char *name);
int get_instruction_funct(char *name);
int is_dest_allowed(char *name, int mode);
int is_src_allowed(char *name, int mode);


#endif
