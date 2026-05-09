CC = gcc
CFLAGS = -ansi -pedantic -Wall #flags to use

assembler: assembler.o const_tables.o first_pass.o machine_image.o output.o pre_proccess.o second_pass.o string_utils.o symbols.o utils.o
	$(CC) $(CFLAGS) assembler.o const_tables.o first_pass.o machine_image.o output.o pre_proccess.o second_pass.o string_utils.o symbols.o utils.o -o assembler

assembler.o: assembler.c
	$(CC) $(CFLAGS) -c assembler.c -o assembler.o

const_tables.o: const_tables.c const_tables.h
	$(CC) $(CFLAGS) -c const_tables.c -o const_tables.o

first_pass.o: first_pass.c first_pass.h
	$(CC) $(CFLAGS) -c first_pass.c -o first_pass.o

machine_image.o: machine_image.c machine_image.h
	$(CC) $(CFLAGS) -c machine_image.c -o machine_image.o

output.o: output.c output.h
	$(CC) $(CFLAGS) -c output.c -o output.o

pre_proccess.o: pre_proccess.c pre_proccess.h
	$(CC) $(CFLAGS) -c pre_proccess.c -o pre_proccess.o

second_pass.o: second_pass.c second_pass.h
	$(CC) $(CFLAGS) -c second_pass.c -o second_pass.o

string_utils.o: string_utils.c string_utils.h
	$(CC) $(CFLAGS) -c string_utils.c -o string_utils.o

symbols.o: symbols.c symbols.h
	$(CC) $(CFLAGS) -c symbols.c -o symbols.o

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c -o utils.o

#clean up the object files
clean:
	rm -f assembler *.o
