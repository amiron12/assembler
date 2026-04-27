#ifndef CONSTANTS_H
#define CONSTANTS_H


#define MACRO_START "mcro"
#define MACRO_END "mcroend"
#define ATTR 4
#define MAX_OPERANDS 80
#define DEC 10
#define MEM_START 100
#define LINE_LENGTH 85
#define MAX_LINE_LENGTH 81
#define ZERO 0
#define ONE 1
#define FALSE 0
#define TRUE 1
#define ERR -1
#define NEG -1

#define AS ".as"
#define AM ".am"
#define OBJ ".ob"
#define ENT ".ent"
#define EXT ".ext"

#define MAX_FILE_NAME 50
#define MAX_ARG_LENGTH 50
#define SYMBOL_LENGTH 35 
#define MAX_SYMBOL_LENGTH 31
#define MEMORY 4096
#define WORD_SIZE 12

#define UNKNOWN '?'
#define ABSOLUTE 'A'
#define RELOCATABLE 'R'
#define EXTERNAL 'E'

#define IC_INDEX (IC)-100
#define MASK 0xFFF
#define MAX_NUM 2047
#define MIN_NUM -2048


typedef enum attribute {data, code, external, entry} attribute;
typedef enum op_mode {IMM, DIR, REL, REG} op_mode;

extern int IC, DC, ICF, DCF;




#endif