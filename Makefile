EXECUTABLE=mips-sim
CC=gcc
CFLAGS=-Wall -c
LDFLAGS=
SOURCES=$(wildcard src/*.c)
OBJECTS=$(wildcard src/*.o, $(SOURCES))

.PHONY: clean

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -rfv src/*.o
