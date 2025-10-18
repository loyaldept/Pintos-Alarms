CC = gcc
CFLAGS = -Wall -g -O -std=gnu99 -I../lib -I../threads -I.

OBJS = timer.o

all: $(OBJS)

timer.o: timer.c timer.h
	$(CC) $(CFLAGS) -c timer.c -o timer.o

clean:
	rm -f *.o
