#ifndef _CPU_H
#define _CPU_H


#include <stddef.h>
#include <stdint.h>

#define NUM_REGISTERS	32

/* Array of register names */
char *reg_names [] = {
	"zero",
	"at",
	"v0",
	"v1",
	"a0",
	"a1",
	"a2",
	"a3",
	"t0",
	"t1",
	"t2",
	"t3",
	"t4",
	"t5",
	"t6",
	"t7",
	"s0",
	"s1",
	"s2",
	"s3",
	"s4",
	"s5",
	"s6",
	"s7",
	"t8",
	"t9",
	"k0",
	"k1",
	"gp",
	"sp",
	"fp",
	"ra",
	"pc",
};

/* Structure of an instruction
 *
 *       31     25    20    15    10     5    0
 *       +------+-----+-----+-----------------+
 *   R   |  op  | rs  | rt  | rd  |shamt|funct|
 *       +------+-----+-----+-----------------+
 *
 *       31     25    20    15                0
 *       +------+-----+-----+-----------------+
 *   I   |  op  | rs  | rt  |    immediate    |
 *       +------+-----+-----+-----------------+
 *
 *       31     25                            0
 *       +------+-----------------------------+
 *   J   |  op  |            Address          |
 *       +------+-----------------------------+
 *
 *
 * The C standard does not guarantee the correct bitfield structure, so the
 * instruction struct has to be split into the 3 format, then union'ed.
 * */
struct instr_r {
	uint8_t funct	: 6;
	uint8_t shamt	: 5;
	uint8_t rd	: 5;
	uint8_t rt	: 5;
	uint8_t rs	: 5;
	uint8_t opcode	: 6;
} __attribute__((packed));

struct instr_i {
	uint16_t immediate;
	uint8_t rt	: 5;
	uint8_t rs	: 5;
	uint8_t opcode	: 6;
} __attribute__((packed));

struct instr_j {
	uint32_t address : 26;
	uint8_t opcode	: 6;
} __attribute__((packed));


typedef union instr {
	struct instr_r r;
	struct instr_i i;
	struct instr_j j;
} instr_t __attribute__((packed));

/*
 * Looks up register name and returns its number.
 * Returns -1 on error. */
int
register_to_number(char *str)
{
	size_t i;
	for(i = 0; i < NUM_REGISTERS; i++) {

	}
}


#endif	/* cpu.h */
