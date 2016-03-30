#ifndef _CPU_H
#define _CPU_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "mips32.h"
#include "cp0.h"

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



/* CPU Structs */
/* Pipeline register */
typedef struct reg_if_id {
	uint32_t inst;
	uint32_t next_pc;
} if_id_t;

typedef struct reg_id_ex {
	/* Control */
	bool c_reg_dst;
	uint8_t c_alu_op : 2;
	bool c_alu_src;
	bool c_branch;
	bool c_mem_read;
	bool c_mem_write;
	bool c_reg_write;
	bool c_mem_to_reg;

	uint8_t shamt : 5;
	uint8_t funct : 6;



	uint32_t next_pc;
	uint32_t rs_value;
	uint32_t rt_value;

	int32_t sign_ext_imm;
	uint8_t rs;
	uint8_t rt;
	uint8_t rd;
} id_ex_t;

typedef struct reg_ex_mem {
	/* Control */
	bool c_reg_write;
	bool c_branch;
	bool c_mem_read;
	bool c_mem_write;
	bool c_mem_to_reg;

	uint32_t branch_target;

	uint32_t eff_addr;
	uint32_t alu_res;

	uint32_t rt_value;

	uint8_t reg_dst;
} ex_mem_t;

typedef struct reg_mem_wb {
	/* Control */
	bool c_reg_write;
	bool c_mem_to_reg;

	uint32_t alu_res;
	uint32_t read_data;

	uint8_t reg_dst;
} mem_wb_t;

/*
 * Core structure of the CPU
 */
typedef struct core {
	uint32_t regs[NUM_REGISTERS];
	cp0_t cp0;

	/* Pipeline registers */
	if_id_t		if_id;
	id_ex_t		id_ex;
	ex_mem_t	ex_mem;
	mem_wb_t	mem_wb;
} core_t;


/*
 * The structure for a complete CPU.
 */
typedef struct cpu {
	core_t *core;	/* The CPU cores. */
	size_t num_cores;
} cpu_t;



/* Initialize a new cpu */
cpu_t *cpu_init(int n_cores);

/* Frees a CPU */
void cpu_free(cpu_t *cpu);

/* Ticks of the clock */
void cpu_tick(cpu_t *cpu);
#endif	/* _CPU_H */
