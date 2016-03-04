AS=mips-elf-as
LD=mips-elf-ld
AFLAGS=-mips32
SRC=$(patsubst %.S,%.elf,$(wildcard *.S))


all: $(SRC)

%.elf: %.S
	$(AS) $(AFLAGS) -o $*.o --fatal-warnings $*.S
	$(LD) -o $*.elf $*.o

clean:
	rm -f *.o *.elf
