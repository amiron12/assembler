all: assembler

assembler: assembler.c pass_one.c symb.c expander.c function_utils.c txt_utils.c tables.c assembler.h

	gcc -g -ansi -pedantic -Wall pass_one.c symb.c assembler.c function_utils.c txt_utils.c expander.c tables.c -o assembler

clean: rm *~