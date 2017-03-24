CC = gcc
LIBS =-I/usr/include/libxml2 -lxml2

program:
	$(CC) $(LIBS) program.c -o program $(LIBS)

parser:
	$(CC) -o parser parser.c $(LIBS)

.PHONY : program parser
