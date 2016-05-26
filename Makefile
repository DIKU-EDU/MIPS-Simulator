TARGET  =./bin/mips-sim
CC=gcc
#CFLAGS  =-g -Wno-parentheses -std=gnu99# -D DEBUG_LEVEL=ERROR_LVL -Wno-packed-bitfield-compat
CFLAGS  =-ggdb -Wall -Werror -Wno-parentheses -std=gnu99 -D DEBUG_LEVEL=ERROR_LVL -Wno-packed-bitfield-compat
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

perf: all perf.gdb ../KUDOS/kudos/kudos-mips32
	gdb -x perf.gdb --args bin/mips-sim -p ../KUDOS/kudos/kudos-mips32

clean:
	rm -f $(OBJECTS) $(TARGET)
