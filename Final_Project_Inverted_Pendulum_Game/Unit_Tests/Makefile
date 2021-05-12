UNAME=$(shell uname)

CCFLAGS=-Wall -O3 -std=gnu99
CC=gcc

all: unit_tests

unit_tests: main.o fifo_queue.o test_functions.o ctest.h unit_tests.o
	$(CC) $(LDFLAGS) main.o fifo_queue.o test_functions.o unit_tests.o -o unittests

remake: clean all

%.o: %.c ctest.h
	$(CC) $(CCFLAGS) -c -o $@ $<

clean:
	rm -f unit_tests *.o
