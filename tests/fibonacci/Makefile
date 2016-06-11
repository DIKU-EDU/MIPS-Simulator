CC=mips-elf-gcc
CFLAGS=-march=mips32 -c
LD=mips-elf-ld
LDFLAGS=--script=ld.script -e main
SRC=$(patsubst %.c,%.elf,$(wildcard *.c))

.DEFAULT: all

all: $(SRC)

%.elf: %.c
	$(CC) $(CFLAGS) -c -o $*.o  $*.c
	$(LD) $(LDFLAGS) -o $*.elf $*.o

clean:
	rm -f *.o *.elf *.out
