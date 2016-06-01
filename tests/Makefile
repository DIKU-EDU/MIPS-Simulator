AS=mips-elf-as
LD=mips-elf-ld
AFLAGS=-mips32 --fatal-warnings
LDFLAGS=--script=ld.script
SRC=$(patsubst %.S,%.elf,$(wildcard *.S))

.DEFAULT: all

all: $(SRC) sos

%.elf: %.S
	$(AS) $(AFLAGS) -o $*.o --fatal-warnings $*.S
	$(LD) $(LDFLAGS) -o $*.elf $*.o


sos:
	make -C sos/

clean:
	rm -f *.o *.elf
