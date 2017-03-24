CC = gcc
CFLAGS = -Wall -std=c11 -g `pkg-config --cflags libxml-2.0` `pkg-config --cflags glib-2.0`
LIBS = `pkg-config --libs libxml-2.0` `pkg-config --libs glib-2.0`
program:
	$(CC) $(CFLAGS) program.c -o program $(LIBS)
