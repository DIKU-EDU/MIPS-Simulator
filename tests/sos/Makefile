TARGET=sos.elf

CC=mips-elf-gcc
AS=mips-elf-as
LD=mips-elf-ld
CFLAGS=-c -ggdb -I. -Wall -Wextra -Werror -std=c99 -pedantic -march=mips32
AFLAGS=-mips32 --fatal-warnings
LDFLAGS=--script=ld.script -Map=sos.map

HEADERS=$(wildcard *.h)

# All of the object files to produce as intermediary work
OBJECTS=$(patsubst %.c, %.o, $(wildcard *.c))
OBJECTS += $(patsubst %.S, %.o, $(wildcard *.S))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $<

%.o: %.S
	$(AS) $(AFLAGS) -o $*.o $*.S


.DEFAULT: all

clean:
	rm -f *.o *.elf $(TARGET)
