all: assembler

assembler: test_utils.c assembler.c bit_utils.c  expander.c function_utils.c machine_image.c macro_utils.c first_pass.c second_pass.c symb.c tables.c txt_utils.c word_utils.c assembler.h

	gcc -g -ansi -pedantic -Wall test_utils.c assembler.c bit_utils.c  expander.c function_utils.c machine_image.c macro_utils.c first_pass.c second_pass.c symb.c tables.c txt_utils.c word_utils.c -o assembler

clean: rm *~


