CC=gcc
CCFLAGS=-Wall
LIBS = -I/usr/include/libxml2 -lxml2
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
TARGET=program

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
