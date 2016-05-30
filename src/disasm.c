#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "mips32.h"
#include "disasm.h"

void print_instruction(uint32_t instr, core_t *core)
{
	char buf[INSTRUCTION_BUFFER_SIZE];
	memset(buf,0, sizeof(buf));

	instruction_string(instr, core, buf, INSTRUCTION_BUFFER_SIZE);

	printf("%s", buf);
}


int instruction_string(uint32_t instr, core_t *core, char *buf, size_t buf_size)
{
	int i = 0; /* bytes written */

	i = snprintf(buf, buf_size - i,"0x%08X\t", instr);


	switch(GET_OPCODE(instr)) {
		/* R-type */
	case OPCODE_R:
		i += snprintf(buf+i, buf_size - i,
			      "%s  rs = %s = 0x%08X,  rt = %s = 0x%08X,  rd = %s = 0x%08X,  shamt = 0x%08X\n",
			      funct_codes[GET_FUNCT(instr)],
			      reg_names[GET_RS(instr)], core->regs[GET_RS(instr)],
			      reg_names[GET_RT(instr)], core->regs[GET_RT(instr)],
			      reg_names[GET_RD(instr)], core->regs[GET_RD(instr)],
			      GET_SHAMT(instr));
		return i;

		/* J-type */
	case OPCODE_J:
	case OPCODE_JAL:
		i += snprintf(buf+i, buf_size - i,
			      "%s   0x%08X\n",
			      op_codes[GET_OPCODE(instr)],
			      GET_IMM(instr));
		return i;

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
	case OPCODE_SB:
	case OPCODE_SC:
	case OPCODE_SH:
	case OPCODE_LBU:
	case OPCODE_LHU:
	case OPCODE_LW:
	case OPCODE_SW:
		i += snprintf(buf+i, buf_size - i,
			      "%s  rs = %s = 0x%08X,  rt = %s = 0x%08X,  imm = 0x%08X\n",
			      op_codes[GET_OPCODE(instr)],
			      reg_names[GET_RS(instr)], core->regs[GET_RS(instr)],
			      reg_names[GET_RT(instr)], core->regs[GET_RT(instr)],
			      GET_IMM(instr));
		return i;


		/* SPECIAL */
	case OPCODE_CP0:
		i += snprintf(buf+i, buf_size - i,
			      "%s, %s  rt = %s = 0x%08X,  rd = %s = 0x%08X\n",
			      op_codes[GET_OPCODE(instr)],
			      cp0_codes[GET_RS(instr)],
			      reg_names[GET_RT(instr)], core->regs[GET_RT(instr)],
			      cp0_reg_names[GET_RD(instr)], core->cp0.regs[GET_RD(instr)]);
		return i;
	default:
		i += snprintf(buf+i, buf_size - i,
			      "Unknown instruction: 0x%08X\n", instr);
	}

	return i;
}
