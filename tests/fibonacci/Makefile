CC=mips-elf-gcc -march=mips32
LD=mips-elf-ld
LDFLAGS=--script=ld.script
SRC=$(patsubst %.c,%.elf,$(wildcard *.c))

.DEFAULT: all

all: $(SRC)

%.elf: %.c
	$(CC) $(CFLAGS) -o $*.o  $*.c
	$(LD) $(LDFLAGS) -o $*.elf $*.o

clean:
	rm -f *.o *.elf
