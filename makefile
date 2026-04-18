CC = gcc
CFLAGS = -g -ansi -pedantic -Wall


SRCS = $(wildcard *.c)
DEPS = $(wildcard *.h)


OBJS = $(SRCS:.c=.o)
TARGET = assembler

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)


%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS) *~

.PHONY: all clean

TODO: refactor