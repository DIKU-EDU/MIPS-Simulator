AS=mips-elf-as
LD=mips-elf-ld
AFLAGS=-mips32
LDFLAGS=--script=ld.script
SRC=$(patsubst %.S,%.elf,$(wildcard *.S))

.DEFAULT: all

all: $(SRC)

%.elf: %.S
	$(AS) $(AFLAGS) -o $*.o --fatal-warnings $*.S
	$(LD) $(LDFLAGS) -o $*.elf $*.o

clean:
	rm -f *.o *.elf
