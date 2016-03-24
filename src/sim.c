#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "tools.h"
#include "sim.h"
#include "cpu.h"
#include "elf.h"
#include "mips32.h"
#include "disasm.h"

#define MEMSZ 0xA0000

/* Signals if program stopped */
static bool finished = false;

/* Signals debugging */
static bool debugging = false;


void interpret_r(uint32_t inst, core_t *core)
{
	switch(GET_FUNCT(inst)) {
	/* Jump register */ case FUNCT_JR:
		core->regs[REG_PC] = core->regs[GET_RS(inst)];

		/* 4 is added later. To negate that: */
		core->regs[REG_PC] -= 4;
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
			(uint32_t)core->regs[GET_RT(inst)] <<
			(uint32_t)GET_SHAMT(inst);
		break;


	case FUNCT_SRL:
		/* Shift Right Logical: rd = rt >> shamt */
		core->regs[GET_RD(inst)] =
			(uint32_t)core->regs[GET_RT(inst)] >>
			(uint32_t)GET_SHAMT(inst);
		break;

	default:
		/* TODO */
		break;
	}

}

void debug(uint32_t inst, core_t* core)
{
	print_instruction(inst, core);

	unsigned char c[3] = {0};
	bool stop = false;

	while(stop == false) {
		printf("> ");
		fgets((char*)c, 3, stdin);

		switch(c[0]) {
			/* Print all registers */
		case 'r':
			print_registers(core);
			break;

			/* Print instruction */
		case 'p':
			print_instruction(inst, core);
			break;

			/* Print specified register only */
		case 'v':
		case 's':
		case 't':
			printf("%s = %u\n", c,
			       core->regs[register_to_number((char*)c)]);
			break;

			/* Continue */
		case 'c':
			stop = true;

		case '\n':
		default:
			break;
		}
	}
	printf("\n");
}


void interpret(core_t *core, memory_t *mem)
{
	uint32_t inst = (uint32_t)GET_BIGWORD(mem->raw, core->regs[REG_PC]);

	uint32_t s_addr = core->regs[GET_RS(inst)] + SIGN_EXTEND(GET_IMM(inst));



	/* Debugging */
	if(debugging)
		debug(inst, core);

	/* Return v0 on SYSCALL */
	if(inst == FUNCT_SYSCALL)
		finished = true;

	/* Interpret instruction accordingly */
	switch(GET_OPCODE(inst)) {
	case OPCODE_R:
		interpret_r(inst, core);
		break;

		/* Jump */
		/* The new address is computed by taking the upper 4 bits of the
		 * PC, concatenated to the 26 bit immediate value, and the lower
		 * two bits are 00, so the address created remains word-aligned.
		 */
	case OPCODE_J:
		core->regs[REG_PC] = (core->regs[REG_PC]
				      & 0xF0000000)
		|(GET_ADDRESS(inst)<<2);

		/* REG_PC will be incremented by 4 later... */
		core->regs[REG_PC] -= 4;
		break;

		/* Jump And Link: RA = PC + 8; PC = Imm;*/
	case OPCODE_JAL:
		core->regs[REG_RA] = core->regs[REG_PC]
		+ 8;

		/* Ordinary Jump */
		core->regs[REG_PC] = (core->regs[REG_PC]
				      & 0xF0000000)
		|(GET_ADDRESS(inst)<<2);

		/* REG_PC will be incremented by 4 later... */
		core->regs[REG_PC] -= 4;
		break;

		/* Branch On Equal: if (RS == RT) { PC = PC + 4 + Imm; } */
	case OPCODE_BEQ:
		if(core->regs[GET_RS(inst)] ==
		   core->regs[GET_RT(inst)])
			core->regs[REG_PC] += (SIGN_EXTEND(
							   GET_IMM(inst))
					       << 2);
		break;

		/* Branch on Not Equal: If (RS != RT) { PC = PC + 4 + Imm} */
	case OPCODE_BNE:
		if(core->regs[GET_RS(inst)] !=
		   core->regs[GET_RT(inst)])
			core->regs[REG_PC] += (SIGN_EXTEND(
							   GET_IMM(inst))
					       << 2);
		break;

		/* Add Immediate: RT = RS + SignExtImm */
	case OPCODE_ADDI:
		core->regs[GET_RT(inst)] =
			core->regs[GET_RS(inst)] +
			SIGN_EXTEND(GET_IMM(inst));
		break;

		/* Add unsigned Immediate: RT = RS + SignExtImm */
	case OPCODE_ADDIU:
		core->regs[GET_RT(inst)] =
			core->regs[GET_RS(inst)] +
			SIGN_EXTEND(GET_IMM(inst));
		break;

		/* Set Less Than Immediate: RT = (RS < SignExtImm) ? 1 : 0 */
	case OPCODE_SLTI:
		core->regs[GET_RT(inst)] =
			(core->regs[GET_RS(inst)] <
			 SIGN_EXTEND((GET_IMM(inst)))) ?
			1 : 0;
		break;

		/* Set Less Than Immediate Unsigned:
		 * RT = (RS < SignExtImm) ? 1 : 0 */
	case OPCODE_SLTIU:
		core->regs[GET_RT(inst)] =
			(core->regs[GET_RS(inst)] <
			 SIGN_EXTEND((GET_IMM(inst)))) ?
			1 : 0;
		break;

		/* And Immediate: RT = RS & ZeroExtImm */
	case OPCODE_ANDI:
		core->regs[GET_RT(inst)] =
			core->regs[GET_RS(inst)] &
			ZERO_EXTEND(GET_IMM(inst));
		break;

		/* Or Immediate: RT = RS | ZeroExtImm */
	case OPCODE_ORI:
		core->regs[GET_RT(inst)] =
			core->regs[GET_RS(inst)]
			| ZERO_EXTEND(GET_IMM(inst));
			break;

			/* Load Upper Immediate: RT = Imm << 16 */
	case OPCODE_LUI:
			core->regs[GET_RT(inst)] =
				((uint32_t)GET_IMM(inst) << 16);
			break;

			/* Load Byte Unsigned: RT = MEM[RS + SignExtImm] */
	case OPCODE_LBU:
			core->regs[GET_RT(inst)] = GET_BIGBYTE(mem->raw,
							       core->regs[GET_RS(inst)]	+
							       SIGN_EXTEND(GET_IMM(inst)));

			break;
			/* Load Halfword Unsigned: RT = MEM[RS + SignExtImm] */
	case OPCODE_LHU:
			core->regs[GET_RT(inst)] = GET_BIGHALF(mem->raw,
							       core->regs[GET_RS(inst)]
							       + SIGN_EXTEND(GET_IMM(inst)));
			break;

			/* Load Word: RT = M[RS + SignExtImm] */
	case OPCODE_LW:
			core->regs[GET_RT(inst)] = GET_BIGWORD(mem->raw,
							       core->regs[GET_RS(inst)]
							       + SIGN_EXTEND(GET_IMM(inst)));

			break;


			/* Load Linked: RT = M[RS + SignExtImm] */
	case OPCODE_LL:
			core->regs[GET_RT(inst)] = GET_BIGWORD(mem->raw,
							       core->regs[GET_RS(inst)]
							       + SIGN_EXTEND(GET_IMM(inst)));

			/* Store the address in CP0 */
			core->cp0.regs[REG_LLADDR] = core->regs[GET_RS(inst)]
			+ SIGN_EXTEND(GET_IMM(inst));

			break;

			/* Store Word: M[RS + SignExtImm] = RT */
	case OPCODE_SW:
			SET_BIGWORD(mem->raw,
				    s_addr,
				    core->regs[GET_RT(inst)]);

			/* If operation overwrites even part of the LLAddr, invalidate*/
			if(abs(core->cp0.regs[REG_LLADDR] - s_addr) < 4)
				core->cp0.regs[REG_LLADDR] = 0xffffffff;
			break;

			/* Store Byte: M[RS + SignExtImm] = RT */
	case OPCODE_SB:
			SET_BIGBYTE(mem->raw,
				    s_addr,
				    core->regs[GET_RT(inst)]);

			/* If operation overwrites even part of the LLAddr, invalidate*/
			if(core->cp0.regs[REG_LLADDR] ==  s_addr)
				core->cp0.regs[REG_LLADDR] = 0xffffffff;

			break;

			/* Store Halfword: M[RS + SignExtImm] = RT */
	case OPCODE_SH:
			SET_BIGHALF(mem->raw,
				    s_addr,
				    core->regs[GET_RT(inst)]);

			/* If operation overwrites even part of the LLAddr, invalidate*/
			if(abs(core->cp0.regs[REG_LLADDR] - s_addr) < 2)
				core->cp0.regs[REG_LLADDR] = 0xffffffff;

			break;

	case OPCODE_SC:
			if(core->cp0.regs[REG_LLADDR] == s_addr) {
				SET_BIGWORD(mem->raw, s_addr, core->regs[GET_RT(inst)]);
				core->regs[GET_RT(inst)] = 1;
			} else {
				core->regs[GET_RT(inst)] = 0;
			}
			break;

			/* SPECIAL OPCODES */
	case OPCODE_CP0:
			/* Function in encoded in RS */
			switch(GET_RS(inst)) {

				/* Move From CP0 */
			case CP0_MFC0:
				core->regs[GET_RT(inst)] = core->cp0.regs[GET_RD(inst)];
				break;

				/* Move To CP0 */
			case CP0_MTC0:
				core->cp0.regs[GET_RD(inst)] = core->regs[GET_RT(inst)];
				break;
			}


	}

	/* Move to next instr */
	core->regs[REG_PC] += 4;
}

void interpret_if()
{
}

void interpret_id()
{
}
void interpret_ex()
{
}
void interpret_mem()
{
}
void interpret_wb()
{
}






int run(hardware_t *hw)
{
	cpu_t* cpu = hw->cpu;
	memory_t* mem = hw->mem;

	while(finished == false) {
		/* Iterate over each core */
		int i;
		for(i = 0; i < cpu->num_cores; i++) {
			interpret(&cpu->core[i], mem);
		}
	}
	return cpu->core[0].regs[REG_V0];
}

int simulate(char *program, bool debug)
{
	/* Set debugging */
	debugging = debug;

	/* Hardware to simulate */
	hardware_t hardware;

	/* Initialize the memory */
	hardware.mem = mem_init(MEMSZ);

	/* Create a new CPU */
	hardware.cpu = cpu_init(1);

	/* Set stack pointer to top of memory */
	hardware.cpu->core[0].regs[REG_SP] = MIPS_RESERVE + MEMSZ - 4;

	/* Load the program into memory */
	if(elf_dump(program, &(hardware.cpu->core[0].regs[REG_PC]),
		    hardware.mem->raw, MEMSZ) != 0) {
		printf("Elf file could not be read.\n");
		exit(0);
	}

	return run(&hardware);
}
