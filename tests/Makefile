AS=mips-elf-as
LD=mips-elf-ld
AFLAGS=-mips32 --fatal-warnings
SRC=$(patsubst %.S,%.elf,$(wildcard *.S))

.DEFAULT: all

all: $(SRC)

%.elf: %.S
	$(AS) $(AFLAGS) -o $*.o --fatal-warnings $*.S
	$(LD) -o $*.elf $*.o

clean:
	rm -f *.o *.elf
