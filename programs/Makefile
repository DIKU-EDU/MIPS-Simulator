AS=mips-elf-as
LD=mips-elf-ld
AFLAGS=-mips32
SRC=$(patsubst %.asm,%.elf,$(wildcard *.asm))


all: $(SRC)

%.elf: %.asm
	$(AS) $(AFLAGS) -o $*.o --fatal-warnings $*.asm
	$(LD) -o $*.elf $*.o

clean:
	rm -f *.o *.elf
