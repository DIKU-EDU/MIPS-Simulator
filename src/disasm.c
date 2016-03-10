#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "mips32.h"
#include "disasm.h"

void print_instruction(uint32_t instr, core_t *core)
{
	switch(GET_OPCODE(instr)) {
		/* R-type */
	case OPCODE_R:
		printf("%s  rs = %s = %d,  rt = %s = %d,  rd = %s = %d,  shamt = %d\n",
		       funct_codes[GET_FUNCT(instr)],
		       reg_names[GET_RS(instr)], core->regs[GET_RS(instr)],
		       reg_names[GET_RT(instr)], core->regs[GET_RT(instr)],
		       reg_names[GET_RD(instr)], core->regs[GET_RD(instr)],
		       GET_SHAMT(instr));
		return;

		/* J-type */
	case OPCODE_J:
	case OPCODE_JAL:
		printf("%s   0x%08X\n",
		       op_codes[GET_OPCODE(instr)],
		       GET_IMM(instr));
		return;

		/* I-type */
	case OPCODE_BEQ:
	case OPCODE_BNE:
	case OPCODE_ADDI:
	case OPCODE_ADDIU:
	case OPCODE_SLTI:
	case OPCODE_SLTIU:
	case OPCODE_ANDI:
	case OPCODE_ORI:
	case OPCODE_LUI:
	case OPCODE_LW:
	case OPCODE_SW:
		printf("%s  rs = %s = %d,  rt = %s = %d,  imm = %d\n",
		       op_codes[GET_OPCODE(instr)],
		       reg_names[GET_RS(instr)], core->regs[GET_RS(instr)],
		       reg_names[GET_RT(instr)], core->regs[GET_RT(instr)],
		       GET_IMM(instr));
		return;

	default:
		printf("Unknown instruction: 0x%08X", instr);
	}

	return;
}
