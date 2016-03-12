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

void interpret_r(uint32_t inst, core_t *core)
{
	switch(GET_FUNCT(inst)) {
	/* Jump register */
	case FUNCT_JR:
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

void debug(uint32_t inst, cpu_t* cpu)
{
	unsigned char c[3] = {0};
	bool stop = false;

	while(stop == false) {
		fgets((char*)c, 3, stdin);

		switch(c[0]) {
			/* Print all registers */
		case 'r':
			print_registers(&cpu->core[0]);
			break;

			/* Print next instruction */
		case 'i':
			print_instruction(inst, &cpu->core[0]);
			break;

		/* Print specified register only */
		case 'v':
		case 's':
		case 't':
			printf("%s = %u\n", c,
			       cpu->core[0].regs[register_to_number((char*)c)]);
			break;

			/* Continue */
		case 'c':
		default:
			stop = true;
			break;
		}
	}
}

int run(hardware_t *hw, bool debugging)
{
	cpu_t* cpu = hw->cpu;
	memory_t* mem = hw->mem;

	while(1) {

		uint32_t inst = 0;
		inst = (uint32_t)GET_BIGWORD(mem->raw, cpu->core[0].regs[REG_PC]);

		/* Debugging */
		if(debugging)
			debug(inst, cpu);



		/* Return v0 on SYSCALL */
		if(inst == FUNCT_SYSCALL)
			return cpu->core[0].regs[REG_V0];

		/* Interpret instruction accordingly */
		switch(GET_OPCODE(inst)) {
		case OPCODE_R:
			interpret_r(inst, &cpu->core[0]);
			break;

			/* Jump */
			/* The new address is computed by taking the upper 4 bits of the
			 * PC, concatenated to the 26 bit immediate value, and the lower
			 * two bits are 00, so the address created remains word-aligned.
			 */
		case OPCODE_J:
			cpu->core[0].regs[REG_PC] = (cpu->core[0].regs[REG_PC]
						     & 0xF0000000)
			|(GET_ADDRESS(inst)<<2);

			/* REG_PC will be incremented by 4 later... */
			cpu->core[0].regs[REG_PC] -= 4;
			break;

			/* Jump And Link: RA = PC + 8; PC = Imm;*/
		case OPCODE_JAL:
			cpu->core[0].regs[REG_RA] = cpu->core[0].regs[REG_PC]
			+ 8;

			/* Ordinary Jump */
			cpu->core[0].regs[REG_PC] = (cpu->core[0].regs[REG_PC]
						     & 0xF0000000)
			|(GET_ADDRESS(inst)<<2);

			/* REG_PC will be incremented by 4 later... */
			cpu->core[0].regs[REG_PC] -= 4;
			break;

			/* Branch On Equal: if (RS == RT) { PC = PC + 4 + Imm; } */
		case OPCODE_BEQ:
			if(cpu->core[0].regs[GET_RS(inst)] ==
			   cpu->core[0].regs[GET_RT(inst)])
				cpu->core[0].regs[REG_PC] += (SIGN_EXTEND(
									  GET_IMM(inst))
							      << 2);
			break;

			/* Branch on Not Equal: If (RS != RT) { PC = PC + 4 + Imm} */
		case OPCODE_BNE:
			if(cpu->core[0].regs[GET_RS(inst)] !=
			   cpu->core[0].regs[GET_RT(inst)])
				cpu->core[0].regs[REG_PC] += (SIGN_EXTEND(
									  GET_IMM(inst))
							      << 2);
			break;

			/* Add Immediate: RT = RS + SignExtImm */
		case OPCODE_ADDI:
			cpu->core[0].regs[GET_RT(inst)] =
				cpu->core[0].regs[GET_RS(inst)] +
				SIGN_EXTEND(GET_IMM(inst));
			break;

			/* Add unsigned Immediate: RT = RS + SignExtImm */
		case OPCODE_ADDIU:
			cpu->core[0].regs[GET_RT(inst)] =
				cpu->core[0].regs[GET_RS(inst)] +
				SIGN_EXTEND(GET_IMM(inst));
			break;

			/* Set Less Than Immediate: RT = (RS < SignExtImm) ? 1 : 0 */
		case OPCODE_SLTI:
			cpu->core[0].regs[GET_RT(inst)] =
				(cpu->core[0].regs[GET_RS(inst)] <
				 SIGN_EXTEND((GET_IMM(inst)))) ?
				1 : 0;
			break;

			/* Set Less Than Immediate Unsigned:
			 * RT = (RS < SignExtImm) ? 1 : 0 */
		case OPCODE_SLTIU:
			cpu->core[0].regs[GET_RT(inst)] =
				(cpu->core[0].regs[GET_RS(inst)] <
				 SIGN_EXTEND((GET_IMM(inst)))) ?
				1 : 0;
			break;

			/* And Immediate: RT = RS & ZeroExtImm */
		case OPCODE_ANDI:
			cpu->core[0].regs[GET_RT(inst)] =
				cpu->core[0].regs[GET_RS(inst)] &
				ZERO_EXTEND(GET_IMM(inst));
			break;

			/* Or Immediate: RT = RS | ZeroExtImm */
		case OPCODE_ORI:
			cpu->core[0].regs[GET_RT(inst)] =
				cpu->core[0].regs[GET_RS(inst)]
				| ZERO_EXTEND(GET_IMM(inst));
				break;

				/* Load Upper Immediate: RT = Imm << 16 */
		case OPCODE_LUI:
				cpu->core[0].regs[GET_RT(inst)] = ((uint32_t)GET_IMM(inst)
								   << 16);
				break;

				/* Load Word: RT = M[RS + SignExtImm] */
		case OPCODE_LW:
				cpu->core[0].regs[GET_RT(inst)] =
					GET_BIGWORD(mem->raw, cpu->core[0]
						    .regs[GET_RS(inst)])
					+ SIGN_EXTEND(GET_IMM(inst));
				break;

				/* Store Word: M[RS + SignExtImm] = RT */
		case OPCODE_SW:
				SET_BIGWORD(mem->raw,
					    cpu->core[0].regs[GET_RS(inst)] +
					    SIGN_EXTEND(GET_IMM(inst)),
					    cpu->core[0].regs[GET_RT(inst)]);
				break;
		}


		/* Move to next instr */
		cpu->core[0].regs[REG_PC] += 4;


	}
}

int simulate(char *program, bool debug)
{
	/* Hardware to simulate */
	hardware_t hardware;

	/* Create a new CPU */
	hardware.cpu = cpu_init(1);

	/* Initialize the memory */
	hardware.mem = mem_init(MEMSZ);

	/* Load the program into memory */
	if(elf_dump(program, &(hardware.cpu->core[0].regs[REG_PC]),
		    hardware.mem->raw, MEMSZ) != 0) {
		printf("Elf file could not be read.\n");
		exit(0);
	}

	return run(&hardware, debug);
}
