CC = gcc
CFLAGS = -Wall
LIBS = -I/usr/include/libxml2 -lxml2

all: program

program:
	$(CC) $(CFLAGS) program.c -o program $(LIBS)

parser:
	$(CC) $(CFLAGS) -o parser parser.c $(LIBS)

clean:
	rm -f *.o *.exe

.PHONY: clean # The .PHONY rule keeps make from doing something with a file named clean
