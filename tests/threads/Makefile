CC = gcc
CFLAGS = -Wall -g -O -std=gnu99 -I../../lib -I../../threads -I../../devices -I.
LDFLAGS = -lm

OBJS = tests.o alarm-single.o alarm-multiple.o alarm-negative.o alarm-zero.o alarm-busy.o

all: $(OBJS)

tests.o: tests.c tests.h
	$(CC) $(CFLAGS) -c tests.c -o tests.o

alarm-single.o: alarm-single.c tests.h
	$(CC) $(CFLAGS) -c alarm-single.c -o alarm-single.o

alarm-multiple.o: alarm-multiple.c tests.h
	$(CC) $(CFLAGS) -c alarm-multiple.c -o alarm-multiple.o

alarm-negative.o: alarm-negative.c tests.h
	$(CC) $(CFLAGS) -c alarm-negative.c -o alarm-negative.o

alarm-zero.o: alarm-zero.c tests.h
	$(CC) $(CFLAGS) -c alarm-zero.c -o alarm-zero.o

alarm-busy.o: alarm-busy.c tests.h
	$(CC) $(CFLAGS) -c alarm-busy.c -o alarm-busy.o

clean:
	rm -f *.o

.PHONY: all clean

