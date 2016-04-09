TARGET  =./bin/mips-sim
CC=gcc
#CFLAGS  =-g -Wno-parentheses -std=gnu99# -D DEBUG_LEVEL=ERROR_LVL -Wno-packed-bitfield-compat
CFLAGS  =-g -Wall -Werror -Wno-parentheses -std=gnu99 -D DEBUG_LEVEL=ERROR_LVL -Wno-packed-bitfield-compat
SRCDIR  = src/
TESTDIR	= tests/
SOURCES =$(wildcard $(SRCDIR)*.c)
OBJECTS =$(SOURCES:.c=.o)
LFLAGS  =

.PHONY: clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	install -D /dev/null $(TARGET)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LFLAGS)

%.o: %.c
	$(CC) -o $@  $(CFLAGS) -c $<

test: $(wildcard $(TESTDIR)*.S)
	make -C $(TESTDIR)

clean:
	rm -f $(OBJECTS) $(TARGET)
