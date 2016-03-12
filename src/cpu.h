#ifndef _CPU_H
#define _CPU_H


#include <stddef.h>
#include <stdint.h>

#include "mips32.h"

/* Registers */
#define NUM_REGISTERS	33		/* PC included as 33 */
extern char *reg_names[NUM_REGISTERS];	/* Defined in cpu.c */
uint32_t register_to_number(char *str);

#define REG_ZERO	0
#define REG_AT		1
#define REG_V0		2
#define REG_V1		3
#define REG_A0		4
#define REG_A1		5
#define REG_A2		6
#define REG_A3		7
#define REG_T0		8
#define REG_T1		9
#define REG_T2		10
#define REG_T3		11
#define REG_T4		12
#define REG_T5		13
#define REG_T6		14
#define REG_T7		15
#define REG_S0		16
#define REG_S1		17
#define REG_S2		18
#define REG_S3		19
#define REG_S4		20
#define REG_S5		21
#define REG_S6		22
#define REG_S7		23
#define REG_T8		24
#define REG_T9		25
#define REG_K0		26
#define REG_K1		27
#define REG_GP		28
#define REG_SP		29
#define REG_FP		30
#define REG_RA		31
#define REG_PC		32





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
typedef union instr_r {
	struct {
		uint8_t funct	: 6;
		uint8_t shamt	: 5;
		uint8_t rd	: 5;
		uint8_t rt	: 5;
		uint8_t rs	: 5;
		uint8_t opcode	: 6;
	}__attribute__((packed));
	uint32_t raw;
}__attribute__((packed)) instr_r_t;

typedef struct instr_i {
	uint16_t immediate;
	uint8_t rt	: 5;
	uint8_t rs	: 5;
	uint8_t opcode	: 6;
}__attribute__((packed)) instr_i_t;

typedef struct instr_j {
	uint32_t address : 26;
	uint8_t opcode	: 6;
}__attribute__((packed)) instr_j_t;


typedef union instr {
	instr_r_t r;
	instr_i_t i;
	instr_j_t j;
	uint32_t raw;
}__attribute__((packed)) instr_t;



/*
 * Core structure of the CPU
 */
typedef struct core {
	uint32_t regs[NUM_REGISTERS];
} core_t;


/*
 * The structure for a complete CPU.
 */
typedef struct cpu {
	core_t *core;	/* The CPU cores. */
} cpu_t;



/* Initialize a new cpu */
cpu_t *cpu_init(int n_cores);

/* Frees a CPU */
void cpu_free(cpu_t *cpu);

/* Ticks of the clock */
void cpu_tick(cpu_t *cpu);

/* Prints all registers of a given core. */
void print_registers(core_t *core);

/* Dump registers except PC in hex. Used for testing */
void dump_registers(core_t *core);

#endif	/* _CPU_H */
