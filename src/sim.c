#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "sim.h"
#include "cpu.h"
#include "elf.h"
#include "mips32.h"


#define MEMSZ 0xA0000
/* Static memory of the CPU. */
uint8_t mem[MEMSZ];

/* TODO */
void dump_mem()
{
	int i;
	for(i = 0; i < MEMSZ; i += 4)
		printf("%d\t%d\t%d\t%d\n", mem[i], mem[i+1], mem[i+2],
		       mem[i+3]);
}

void interpret_r(uint32_t inst, core_t *core)
{
	switch(GET_FUNCT(inst)) {
	case FUNCT_JR:
		/* TODO: Jump register */
		break;
	case FUNCT_SYSCALL:
		/* TODO: System call */
		break;
	case FUNCT_ADD:
		/* Signed addition */
		core->regs[GET_RD(inst)] =
			(int32_t)core->regs[GET_RS(inst)] +
			(int32_t)core->regs[GET_RT(inst)];
		break;
	case FUNCT_ADDU:
		/* Unsigned addition */
		core->regs[GET_RD(inst)] =
			(uint32_t)core->regs[GET_RS(inst)] +
			(uint32_t)core->regs[GET_RT(inst)];
		break;
	case FUNCT_SUB:
		/* Signed subtraction */
		core->regs[GET_RD(inst)] =
			(int32_t)core->regs[GET_RS(inst)] -
			(int32_t)core->regs[GET_RT(inst)];

		break;

	case FUNCT_SUBU:
		/* Unsigned subtraction */
		core->regs[GET_RD(inst)] =
			(uint32_t)core->regs[GET_RS(inst)] -
			(uint32_t)core->regs[GET_RT(inst)];
		break;

	case FUNCT_AND:
		/* Bitwise and */
		core->regs[GET_RD(inst)] =
			(uint32_t)core->regs[GET_RS(inst)] &
			(uint32_t)core->regs[GET_RT(inst)];
		break;

	case FUNCT_OR:
		/* Bitwise or */
		core->regs[GET_RD(inst)] =
			(uint32_t)core->regs[GET_RS(inst)] |
			(uint32_t)core->regs[GET_RT(inst)];
		break;

	case FUNCT_NOR:
		/* Bitwise nor (!(a | b)) */
		core->regs[GET_RD(inst)] =
			(!(uint32_t)core->regs[GET_RS(inst)] |
			(uint32_t)core->regs[GET_RT(inst)]);
		break;

	case FUNCT_SLT:
		/* Signed Set Less Than: rd = rs < rt ? 1 : 0*/
		core->regs[GET_RD(inst)] =
			(int32_t)core->regs[GET_RS(inst)] <
			(int32_t)core->regs[GET_RT(inst)] ? 1 : 0;
		break;


	case FUNCT_SLTU:
		/* Unsigned Set Less Than: rd = rs < rt ? 1 : 0*/
		core->regs[GET_RD(inst)] =
			(uint32_t)core->regs[GET_RS(inst)] <
			(uint32_t)core->regs[GET_RT(inst)] ? 1 : 0;
		break;


	case FUNCT_SLL:
		/* Shift Left Logical: rd = rt << shamt */
		core->regs[GET_RD(inst)] =
			(uint32_t)core->regs[GET_RS(inst)] <<
			(uint32_t)GET_SHAMT(inst);
		break;


	case FUNCT_SRL:
		/* Shift Right Logical: rd = rt >> shamt */
		core->regs[GET_RD(inst)] =
			(uint32_t)core->regs[GET_RS(inst)] >>
			(uint32_t)GET_SHAMT(inst);
		break;

	default:
		/* TODO */
		break;
	}

}


int run(cpu_t* cpu)
{
	while(1) {
		printf("PC: %d\n", cpu->core[0].regs[31]);

		uint32_t inst = 0;
		inst = (uint32_t)GET_BIGWORD(mem, cpu->core[0].regs[31]);

		/*
		printf("opcode: %d\trd: %d\trs: %d\trt: %d\t\n",
		       GET_OPCODE(inst), GET_RD(inst), GET_RS(inst),
		       GET_RT(inst));
		*/

		/* Interpret instruction accordingly */
		switch(GET_OPCODE(inst)) {
		case OPCODE_R:
			interpret_r(inst, &cpu->core[0]);
		}


		/* Move to next instr */
		cpu->core[0].regs[32] += 4;

		if('s' == getchar()) /* Pause */
			print_registers(&cpu->core[0]);

	}
}


int simulate(char *program)
{
	/* Create a new CPU */
	cpu_t *cpu = cpu_init(1);

	/* Load the program into memory */
	if(elf_dump(program, &(cpu->core[0].regs[31]), mem, MEMSZ) != 0) {
		printf("Elf file could not be read.\n");
		exit(0);
	}

	run(cpu);
	return 0;
}
