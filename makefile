CC = gcc
CFLAGS = -Wall
LIBS = -I/usr/include/libxml2 -lxml2

all: program

program: interface.o parser.o
	$(CC) $(CFLAGS) interface.o parser.o program.c $(LIBS)

interface.o: interface.c
	$(CC) $(CFLAGS) interface.c $(LIBS)

parser.o: parser.c
	$(CC) $(CFLAGS) parser.c $(LIBS)

clean:
	rm -f *.o *.exe

.PHONY: clean # The .PHONY rule keeps make from doing something with a file named clean
