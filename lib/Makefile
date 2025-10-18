CC = gcc
CFLAGS = -Wall -g -O -std=gnu99 -I.

OBJS = debug.o

all: $(OBJS)

debug.o: debug.c debug.h
	$(CC) $(CFLAGS) -c debug.c -o debug.o

clean:
	rm -f *.o

.PHONY: all clean