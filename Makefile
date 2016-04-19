TARGET  =./bin/mips-sim
CC=gcc
#CFLAGS  =-g -Wno-parentheses -std=gnu99# -D DEBUG_LEVEL=ERROR_LVL -Wno-packed-bitfield-compat
CFLAGS  =-g -Wall -Werror -Wno-parentheses -std=gnu11 -D DEBUG_LEVEL=ERROR_LVL -Wno-packed-bitfield-compat
# CFLAGS  =-E -dD
SRCDIR  = src/
TESTDIR	= tests/
SOURCES =$(wildcard $(SRCDIR)*.c)
OBJECTS =$(SOURCES:.c=.o)
LFLAGS  =

.PHONY: clean all

all: $(TARGET)

$(TARGET): $(OBJECTS) Makefile
	install -D /dev/null $(TARGET)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LFLAGS)

%.o: %.c Makefile
	$(CC) -o $@  $(CFLAGS) -c $<

test: $(wildcard $(TESTDIR)*.S) Makefile
	make -C $(TESTDIR)

clean:
	rm -f $(OBJECTS) $(TARGET)
