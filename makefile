CC = gcc
CFLAGS = -g -ansi -pedantic -Wall
SRCS = $(wildcard *.c)
TARGET = assembler

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET) *~

.PHONY: all clean