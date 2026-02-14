all: assembler

assembler: assembler.c expander.c tables.c assembler.h

	gcc -g -ansi -pedantic -Wall assembler.c expander.c tables.c -o assembler

clean: rm *~