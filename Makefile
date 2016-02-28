TARGET  =./bin/mips-sim
CC=gcc
CFLAGS  =-g -Wall -Werror -Wno-parentheses -std=gnu99 -D DEBUG_LEVEL=ERROR_LVL -Wno-packed-bitfield-compat
SRCDIR  = src/
SOURCES =$(wildcard $(SRCDIR)*.c)
OBJECTS =$(SOURCES:.c=.o)
LFLAGS  =

.PHONY: clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LFLAGS)

%.o: %.c
	$(CC) -o $@  $(CFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)
