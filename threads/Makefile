CC = gcc
CFLAGS = -Wall -g -O -std=gnu99 -I../lib -I. -I../devices
LDFLAGS = -lm

OBJS = thread.o

all: $(OBJS)

thread.o: thread.c thread.h debug.h list.h interrupt.h synch.h flags.h intr-stubs.h palloc.h switch.h vaddr.h
	$(CC) $(CFLAGS) -c thread.c -o thread.o

clean:
	rm -f *.o