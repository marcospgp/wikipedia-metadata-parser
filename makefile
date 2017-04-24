CC = gcc
CCFLAGS = -O2 -Wall -std=c11 -g `pkg-config --cflags libxml-2.0` `pkg-config --cflags glib-2.0`
LIBS =  `pkg-config --libs libxml-2.0` `pkg-config --libs glib-2.0`
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
TARGET = program

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LIBS)

%.o: %.c %.h
	$(CC) $(CCFLAGS) -c $< $(LIBS)

%.o: %.c
	$(CC) $(CCFLAGS) -c $< $(LIBS)

clean:
	rm -f *.o $(TARGET)

.PHONY: clean # The .PHONY rule keeps make from doing something with a file named clean
