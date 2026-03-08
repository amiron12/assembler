#ifndef CONSTANTS_H
#define CONSTANTS_H

/* Values used in pre-proccess */
#define MACRO_START "mcro"
#define MACRO_END "mcroend"

#define DEC 10
#define BYTE (sizeof(unsigned char)*8)
#define MEM_START 100
#define MAX_LINE_LENGTH 80 /* including new-line character */
#define MAX_FNAME 15
#define ZERO 0
#define ONE 1
#define FALSE 0
#define TRUE 1
#define ERR -1
#define NEG -1
#define OK 1
#define AS ".as"
#define AM ".am"
#define OBJ ".ob"
#define ENT ".ent"
#define EXT ".ext"
#define MAX_ARG_LENGTH 30
#define LABEL_LENGTH 31 /* TODO: make sure its the right length with \n and so */
#define MEMORY 4096
#define WORD_SIZE 12
#define FILE_NAME 30
#define UNKNOWN '?'
#define ABSOLUTE 'A'
#define RELOCATABLE 'R'
#define EXTERNAL 'E'
#define ICINDEX (IC)-100
#define MASK 0xFFF

typedef enum attribute {data, code, external, entry} attribute;
typedef enum line_type {directive, instruction} line_type;
typedef enum op_mode {IMM, DIR, REL, REG} op_mode;

int IC, DC, ICF, DCF;


#endif