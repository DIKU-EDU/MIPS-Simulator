#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "tools.h"
#include "sim.h"
#include "cpu.h"
#include "io.h"
#include "elf.h"
#include "mips32.h"
#include "disasm.h"
#include "debug.h"
#include "error.h"
#include "shutdown_dev.h"


/* MACROS for less typing */
#define IF_ID (core->if_id)
#define ID_EX (core->id_ex)
#define EX_MEM (core->ex_mem)
#define MEM_WB (core->mem_wb)
#define PC (core->regs[REG_PC])
#define REGS(x) (core->regs[(x)])


/* Signals if program stopped */
bool g_finished = false;

/* Signals debugging */
bool g_debugging = false;


void interpret_if(core_t *core, mmu_t *mem)
{
	/* Fetch the next instruction */
	uint32_t inst = 0;
	IF_ID.exception = mmu_read(core, mem, REGS(REG_PC), &inst,
				   MEM_OP_WORD);

	/* Hazard control
	 * COD5 p. 314 */
	if(ID_EX.c_mem_read
	   && ((ID_EX.rt == GET_RS(inst))
	       || (ID_EX.rt == GET_RT(inst)))) {
		/* Insert NOP */
		IF_ID.inst = 0;
		return;
	}

	core->if_id.inst = inst;

	/* Point PC to next instruction and store in pipeline reg */
	PC += 4;
	IF_ID.next_pc = PC;
	IF_ID.BadVAddr = 0;

	IF_ID.is_branch_delay = false;
}

/* Control unit in the ID stage */
void interpret_id_control(core_t *core)
{
	uint32_t inst = IF_ID.inst;

	/* Clear all */
	ID_EX.c_reg_dst		= 0;
	ID_EX.c_alu_op		= 0x00;
	ID_EX.c_alu_src		= 0;
	ID_EX.c_beq		= 0;
	ID_EX.c_bne		= 0;
	ID_EX.c_mem_read	= 0;
	ID_EX.c_mem_write	= 0;
	ID_EX.c_reg_write	= 0;
	ID_EX.c_mem_to_reg	= 0;
	ID_EX.c_jump		= 0;


	/* COD5, page 302, fig. 4.49 */
	switch(GET_OPCODE(inst))
	{
	case OPCODE_R:
		/* If JR */
		if(GET_FUNCT(ID_EX.inst) == FUNCT_JR) {
			ID_EX.c_jump		= 1;
		} else {
			ID_EX.c_reg_dst		= 1;
			ID_EX.c_alu_op		= 0x02;
			ID_EX.c_reg_write	= 1;
		}
		break;

	case OPCODE_LWL:
	case OPCODE_LWR:
	case OPCODE_LBU:
	case OPCODE_LHU:
	case OPCODE_LW:
		ID_EX.c_alu_src		= 1;
		ID_EX.c_mem_read	= 1;
		ID_EX.c_reg_write	= 1;
		ID_EX.c_mem_to_reg	= 1;
		break;

	case OPCODE_LL:
	case OPCODE_LUI:
		ID_EX.c_alu_op		= 0x03;
		ID_EX.c_alu_src		= 1;
		ID_EX.c_reg_write	= 1;
		ID_EX.c_reg_dst		= 0; /* write to RT */
		break;


	case OPCODE_SC:
		DEBUG("SC Instruction not fully implemented. Storing only.");

	case OPCODE_SWR:
	case OPCODE_SWL:
	case OPCODE_SB:
	case OPCODE_SH:
	case OPCODE_SW:
		ID_EX.c_alu_op		= 0x00;
		ID_EX.c_alu_src		= 1;
		ID_EX.c_mem_write	= 1;
		break;


	case OPCODE_BEQ:
		ID_EX.c_beq		= 1;
		break;

	case OPCODE_BNE:
		ID_EX.c_bne		= 1;
		break;

	case OPCODE_SLTI:
	case OPCODE_SLTIU:
	case OPCODE_ORI:
	case OPCODE_ANDI:
	case OPCODE_ADDI:
	case OPCODE_ADDIU:
		ID_EX.c_alu_op		= 0x03;
		ID_EX.c_alu_src		= 1;
		ID_EX.c_reg_write	= 1;
		break;


	case OPCODE_J:
		ID_EX.c_jump		= 1;
		ID_EX.jump_addr = (ID_EX.next_pc & 0xF0000000)
			| (GET_ADDRESS(ID_EX.inst)<<2);
		break;

		/* Same as JUMP, but store the next address in $ra */
	case OPCODE_JAL:
		ID_EX.c_jump		= 1;
		ID_EX.jump_addr = (ID_EX.next_pc & 0xF0000000)
			| (GET_ADDRESS(ID_EX.inst)<<2);

		/* Value to store */
		ID_EX.rs_value = ID_EX.next_pc + 0x04; /* after branch delay */

		/* Save to RT register */
		ID_EX.c_reg_dst = 0;

		/* RT is return address register */
		ID_EX.rt = REG_RA;

		/* Write to register, of course */
		ID_EX.c_reg_write = 1;
		break;

		/* NOTE: These instructions cannot be executed in the same clock, due
		 * data-hazards.
		 *
		 * These functions work by adding the source register with $0, saving
		 * to the specified register (basically converting instruction to ADD).
		 */
	case OPCODE_CP0:
		/* Function in encoded in RS */
		switch(GET_RS(inst)) {
			/* Move From CP0 */
		case CP0_MFC0:
			ID_EX.c_reg_dst		= 1; /* Write to RD */
			ID_EX.c_alu_op		= 0x02; /* R TYPE */
			ID_EX.c_reg_write	= 1;


			ID_EX.rs = GET_RD(inst) + 1;
			ID_EX.rs_value = core->cp0.regs[GET_RD(inst)+1];

			ID_EX.rt = 0;
			ID_EX.rt_value = 0;

			ID_EX.rd = GET_RT(inst);

			/* Set ADD function */
			ID_EX.funct = FUNCT_ADD;
			break;

			/* Move To CP0 */
		case CP0_MTC0:
			ID_EX.c_reg_dst		= 1; /* Write to RD */
			ID_EX.c_alu_op		= 0x02; /* R TYPE */
			ID_EX.c_reg_write	= 1;


			ID_EX.rs = GET_RT(inst);
			ID_EX.rs_value = core->regs[GET_RT(inst)];

			ID_EX.rt = 0;
			ID_EX.rt_value = 0;

			/* CP0 registers are offset by 1 due to forwarding
			 * trouble. */
			ID_EX.rd		= GET_RD(inst) + 1;

			/* Set ADD function */
			ID_EX.funct = FUNCT_ADD;
			break;
		}

	}

}

void interpret_id(core_t *core)
{
	uint32_t inst = IF_ID.inst;

	ID_EX.rt		= GET_RT(inst);
	ID_EX.rd		= GET_RD(inst);
	ID_EX.rs		= GET_RS(inst);
	ID_EX.rs_value		= core->regs[GET_RS(inst)];
	ID_EX.rt_value		= core->regs[GET_RT(inst)];
	ID_EX.sign_ext_imm	= SIGN_EXTEND(GET_IMM(inst));
	ID_EX.funct		= GET_FUNCT(inst);
	ID_EX.shamt		= GET_SHAMT(inst);
	ID_EX.inst		= IF_ID.inst;
	ID_EX.next_pc		= IF_ID.next_pc;
	ID_EX.exception		= IF_ID.exception;
	ID_EX.BadVAddr		= IF_ID.BadVAddr;
	ID_EX.is_branch_delay	= IF_ID.is_branch_delay;

	ID_EX.exception = EXC_None;

	/* Forward exception and return, if any */
	if(IF_ID.exception != EXC_None) {
		ID_EX.exception = IF_ID.exception;
		ID_EX.BadVAddr = IF_ID.BadVAddr;
		return;
	}

	/* Control unit */
	interpret_id_control(core);
}

/* ALU in EX stage
 * COD5, page 301, fig. 4.47 */
void interpret_ex_alu(core_t *core)
{
	uint32_t a = ID_EX.rs_value;

	/* MUX B for alu_src */
	uint32_t b = ID_EX.c_alu_src == 0 ? ID_EX.rt_value : ID_EX.sign_ext_imm ;

	/* LW and SW */
	if(ID_EX.c_alu_op == 0x00) {
		/* Check for overflow */
		if(check_soverflow((int32_t)a,(int32_t)b) == 1) {
			DEBUG("OVERFLOW 0x%08x + 0x%08x", a, b);
			EX_MEM.exception = EXC_ArithmeticOverflow;
		}

		EX_MEM.alu_res = (int32_t)a + (int32_t)b;
		return;
	}

	/* BEQ */
	if(ID_EX.c_alu_op == 0x01) {
		/* UNUSED */
		return;
	}

	/* R-Type */
	if(ID_EX.c_alu_op == 0x02) {
		switch(ID_EX.funct) {
		case FUNCT_ADD:
			DEBUG("OVERFLOW 0x%08x + 0x%08x", a, b);
			EX_MEM.alu_res = (int32_t)a + (int32_t)b;
			break;

		case FUNCT_ADDU:
			EX_MEM.alu_res = a + b;
			break;

		case FUNCT_SUB:
			EX_MEM.alu_res = (int32_t)a + (int32_t)b;
			break;

		case FUNCT_SUBU:
			EX_MEM.alu_res = a - b;
			break;

		case FUNCT_AND:
			EX_MEM.alu_res = a & b;
			break;

		case FUNCT_OR:
			EX_MEM.alu_res = a | b;
			break;

		case FUNCT_NOR:
			EX_MEM.alu_res = ~(uint32_t)((uint32_t)a | (uint32_t)b);
			break;

		case FUNCT_SLT:
			EX_MEM.alu_res = ((int32_t)a < (int32_t)b) ? 1 : 0;
			break;

		case FUNCT_SLTU:
			EX_MEM.alu_res = a < b ? 1 : 0;
			break;

		case FUNCT_SLL:
			EX_MEM.alu_res = b << ID_EX.shamt;
			break;

		case FUNCT_SRL:
			EX_MEM.alu_res = b >> ID_EX.shamt;
			break;

		case FUNCT_SYSCALL:
			EX_MEM.exception = EXC_Syscall;
			break;
		default:
			ERROR("Unknown funct: 0x%x", ID_EX.funct);
			print_instruction(ID_EX.inst, core);
			break;
		}
	}

	/* I-Type */
	if(ID_EX.c_alu_op == 0x03) {
		switch(GET_OPCODE(ID_EX.inst)) {
		case OPCODE_ADDI:
			/* Check for overflow */
			if(check_soverflow(a,b) == 1) {
				DEBUG("OVERFLOW 0x%08x + 0x%08x", a,b);
				EX_MEM.exception = EXC_ArithmeticOverflow;
			}
			EX_MEM.alu_res = (int32_t)a + (int32_t)b;
			break;

		case OPCODE_ADDIU:
			EX_MEM.alu_res = a + b;
			break;

		case OPCODE_SLTI:
			EX_MEM.alu_res = (int32_t)a < (int32_t) b ? 1 : 0;
			break;

		case OPCODE_SLTIU:
			EX_MEM.alu_res = (uint32_t)a < (uint32_t) b ? 1 : 0;

			break;

		case OPCODE_ANDI:
			EX_MEM.alu_res = a & (b & ZERO_EXTEND_MASK);
			break;

		case OPCODE_ORI:
			EX_MEM.alu_res = a | (b & ZERO_EXTEND_MASK);
			break;

		case OPCODE_LUI:
			EX_MEM.alu_res = (uint32_t)b << 16;
			break;
		}
	}
}

void interpret_ex(core_t *core)
{
	/* Pipe to next pipeline registers */
	/* MEM */
	EX_MEM.c_mem_read	= ID_EX.c_mem_read;
	EX_MEM.c_mem_write	= ID_EX.c_mem_write;
	EX_MEM.rt_value		= ID_EX.rt_value;
	/* WB */
	EX_MEM.c_reg_write	= ID_EX.c_reg_write;
	EX_MEM.c_mem_to_reg	= ID_EX.c_mem_to_reg;

	EX_MEM.inst		= ID_EX.inst;
	EX_MEM.next_pc		= ID_EX.next_pc;

	EX_MEM.is_branch_delay	= ID_EX.is_branch_delay;



	EX_MEM.exception = EXC_None;

	/* Forward exception and return, if any */
	if(ID_EX.exception != EXC_None) {
		EX_MEM.exception = ID_EX.exception;
		EX_MEM.BadVAddr = ID_EX.BadVAddr;
		return;
	}


	/* On J and JR */
	if(ID_EX.c_jump == 1) {
		if(GET_OPCODE(ID_EX.inst) == OPCODE_R
		   && GET_FUNCT(ID_EX.inst) == FUNCT_JR
		   || ID_EX.inst == INSTRUCTION_ERET) {
			ID_EX.jump_addr = ID_EX.rs_value;
		}

		/* Previous instruction is branch delay */
		IF_ID.is_branch_delay = true;

		DEBUG("JUMPING TO: %08x", ID_EX.jump_addr);
		PC = ID_EX.jump_addr;
	}
	/* On BEQ */
	if(ID_EX.c_beq == 1) {
		if(ID_EX.rs_value == ID_EX.rt_value) {
			/* Calculate branch address */
			PC = ID_EX.next_pc + (ID_EX.sign_ext_imm << 2);
			DEBUG("BRANCHING TO: %08x", PC);


			/* Previous instruction is branch delay */
			IF_ID.is_branch_delay = true;
		}
	}
	/* On BNE */
	if(ID_EX.c_bne == 1) {
		if(ID_EX.rs_value != ID_EX.rt_value) {
			/* Calculate branch address */
			PC = ID_EX.next_pc + (ID_EX.sign_ext_imm << 2);
			DEBUG("BRANCHING TO: %08x", PC);

			/* Previous instruction is branch delay */
			IF_ID.is_branch_delay = true;
		}
	}

	/* MUX for RegDST */
	EX_MEM.reg_dst = ID_EX.c_reg_dst == 0 ? ID_EX.rt : ID_EX.rd;

	/* ALU */
	interpret_ex_alu(core);
}

void interpret_mem(core_t *core, mmu_t *mem)
{
	MEM_WB.c_reg_write	= EX_MEM.c_reg_write;
	MEM_WB.reg_dst		= EX_MEM.reg_dst;
	MEM_WB.c_mem_to_reg	= EX_MEM.c_mem_to_reg;
	MEM_WB.alu_res		= EX_MEM.alu_res;

	MEM_WB.inst		= EX_MEM.inst;
	MEM_WB.next_pc		= EX_MEM.next_pc;
	MEM_WB.exception	= EX_MEM.exception;
	MEM_WB.is_branch_delay	= EX_MEM.is_branch_delay;


	MEM_WB.exception = EXC_None;
	/* Forward exception and return, if any */
	if(EX_MEM.exception != EXC_None) {
		MEM_WB.exception = EX_MEM.exception;
		MEM_WB.BadVAddr = EX_MEM.BadVAddr;
		return;
	}

	/* If read */
	if(EX_MEM.c_mem_read) {
		switch(GET_OPCODE(MEM_WB.inst)) {
		case OPCODE_LW:
			MEM_WB.exception = mmu_read(core, mem, EX_MEM.alu_res,
						    &MEM_WB.read_data,
						    MEM_OP_WORD);

			break;
		case OPCODE_LBU:
			MEM_WB.exception = mmu_read(core, mem, EX_MEM.alu_res,
						    &MEM_WB.read_data,
						    MEM_OP_BYTE);
			break;
		case OPCODE_LHU:
			MEM_WB.exception = mmu_read(core, mem, EX_MEM.alu_res,
						    &MEM_WB.read_data,
						    MEM_OP_HALF);
			break;


			/* From yams */
		case OPCODE_LWL:
			{
				/* Read the word to temp32 */
				uint32_t temp32 = 0x00;
				MEM_WB.exception = mmu_read(core,
							    mem,
							    EX_MEM.alu_res & 0xfffffffc,
							    &temp32,
							    MEM_OP_WORD);

				uint32_t mask;
				uint32_t shift;

				temp32 = temp32 << ((EX_MEM.alu_res & 0x00000003)
						    *8);

				shift = 32-(EX_MEM.alu_res & 3)*8;
				mask = ((uint32_t)0xffffffff) >> shift;
				if(shift >= 32) mask = 0x00000000;

				MEM_WB.read_data = core->regs[EX_MEM.reg_dst]
					& mask
					| temp32;
			}
			break;

			/* From yams */
		case OPCODE_LWR:
			{
				/* Read the word to temp32 */
				uint32_t temp32 = 0x00;
				MEM_WB.exception = mmu_read(core,
							    mem,
							    EX_MEM.alu_res & 0xfffffffc,
							    &temp32,
							    MEM_OP_WORD);

				uint32_t mask;
				uint32_t shift;

				temp32 = temp32 >> (24-(EX_MEM.alu_res & 0x00000003)*8);

				shift = 8+(EX_MEM.alu_res & 3)*8;
				mask = ((uint32_t)0xffffffff) << shift;
				if(shift >= 32) mask = 0x00000000;

				MEM_WB.read_data = core->regs[EX_MEM.reg_dst] & mask |
					temp32;
			}
			break;
		}

		/* If write */
	} else if(EX_MEM.c_mem_write) {
		switch(GET_OPCODE(MEM_WB.inst)) {
		case OPCODE_SW:
			MEM_WB.exception  = mmu_write(core, mem, EX_MEM.alu_res,
						      EX_MEM.rt_value,
						      MEM_OP_WORD);
			break;
		case OPCODE_SB:
			MEM_WB.exception  = mmu_write(core, mem, EX_MEM.alu_res,
						      EX_MEM.rt_value,
						      MEM_OP_BYTE);
			break;
		case OPCODE_SH:
			MEM_WB.exception  = mmu_write(core, mem, EX_MEM.alu_res,
						      EX_MEM.rt_value,
						      MEM_OP_HALF);
			break;
			/* From yams */
		case OPCODE_SWL:
			{
				/* Read the word to temp32 */
				uint32_t temp32 = 0x00;
				MEM_WB.exception = mmu_read(core,
							    mem,
							    EX_MEM.alu_res & 0xfffffffc,
							    &temp32,
							    MEM_OP_WORD);

				uint32_t shift = 32-(EX_MEM.alu_res & 3) * 8;
				uint32_t mask = ((uint32_t)0xffffffff) << shift;
				if(shift >= 32) mask = 0x00000000;

				temp32 = temp32 & mask;
				temp32 = temp32
					| core->regs[GET_RT(EX_MEM.inst)]
					>> ((EX_MEM.alu_res & 3) * 8);




				temp32 = temp32 >> (24-(EX_MEM.alu_res & 0x00000003)*8);

				shift = 8+(EX_MEM.alu_res & 3)*8;
				mask = ((uint32_t)0xffffffff) << shift;
				if(shift >= 32) mask = 0x00000000;

				MEM_WB.exception  = mmu_write(core,
							      mem,
							      EX_MEM.alu_res & 0xfffffffc,
							      temp32,
							      MEM_OP_WORD);
			}
			break;


			/* From yams */
		case OPCODE_SWR:
			{
				/* Read the word to temp32 */
				uint32_t temp32 = 0x00;
				MEM_WB.exception = mmu_read(core,
							    mem,
							    EX_MEM.alu_res & 0xfffffffc,
							    &temp32,
							    MEM_OP_WORD);

				uint32_t shift = 8+(EX_MEM.alu_res & 3) * 8;

				uint32_t mask = ((uint32_t)0xffffffff) >> shift;

				if(shift >= 32) mask = 0x00000000;

				temp32 = temp32 & mask;
				temp32 = temp32
					| core->regs[GET_RT(EX_MEM.inst)]
					<< (24-(EX_MEM.alu_res & 3) * 8);

				MEM_WB.exception  = mmu_write(core,
							      mem,
							      EX_MEM.alu_res & 0xfffffffc,
							      temp32,
							      MEM_OP_WORD);
			}
			break;

		}
	}
}
/* NOTE: This does not use pipelined approach, and so, theoretically skips a
 * few clock cycles. */
void handle_exception(core_t *core, mmu_t *mem)
{
	/* 0. Clear the pipeline, reverting all instructions in the pipeline */
	bzero(&IF_ID, sizeof(struct reg_if_id));
	bzero(&ID_EX, sizeof(struct reg_id_ex));
	bzero(&EX_MEM, sizeof(struct reg_ex_mem));

	/* Data in MEM_WB is still needed */

	/* 1. Save EPC */
	/* PC is inaccessible using regular instruction encoding. We will just
	 * assign it directly, which will skip a few clocks. Hack? */
	core->cp0.regs[REG_EPC] = MEM_WB.next_pc; /* next_pc points to next
						     instruction, so subtract 4.*/


	DEBUG("EPC = 0x%08x", core->cp0.regs[REG_EPC]);

	/* 2. Save CAUSE. If Address exception, save failing addr in BadVAddr */
	uint32_t cause = get_cause(MEM_WB.exception, MEM_WB.is_branch_delay);
	core->cp0.regs[REG_CAUSE] = cause;

	if(MEM_WB.exception == EXC_AddressErrorLoad
	   || MEM_WB.exception == EXC_AddressErrorLoad) {
		core->cp0.regs[REG_BADVADDR] = MEM_WB.BadVAddr;
	}


	/* 3. Set SR(EXL) to set CPU into kernel mode and disable interrupts */
	core->cp0.regs[REG_SR] |= SR_EXL;

	/* 4. Jump to exception handler at 0x80000080, or to EBASE, if SR_BEV
	 * is set */
	core->regs[REG_PC] = (uint32_t)0x80000080;

	if(core->cp0.regs[REG_SR] & SR_BEV) {
		core->regs[REG_PC] = core->cp0.regs[REG_EBASE];
	}

	/* Clear last stage */
	bzero(&MEM_WB, sizeof(struct reg_mem_wb));
}

void interpret_wb(core_t *core, mmu_t *mem)
{
	/* Special instructions */
	/* ERET could be handled earlier in the stage, but adds too much
	 * additional complexity to the code. */
	if(MEM_WB.inst == INSTRUCTION_ERET) {
		/* Flush rest of the pipeline */
		bzero(&IF_ID, sizeof(struct reg_if_id));
		bzero(&ID_EX, sizeof(struct reg_id_ex));
		bzero(&EX_MEM, sizeof(struct reg_ex_mem));


		core->regs[REG_PC] = core->cp0.regs[REG_EPC];

		/* Reset EXL bit */
		core->cp0.regs[REG_SR] &= ~SR_EXL;

		DEBUG("ERET CAUGHT. Jump destination: 0x%08x",
		      core->regs[REG_PC]);

		/* Clear-out the rest */
		bzero(&MEM_WB, sizeof(struct reg_mem_wb));

		/* Do the return */
		return;
	}


	/* Check for any exceptions */
	if(MEM_WB.exception != EXC_None) {
		DEBUG("Exception %s caught.", exc_names[MEM_WB.exception]);

		/* XXX: Exit on Syscall, v0 = 10. Temporary */
		if(MEM_WB.exception == EXC_Syscall
		   && core->regs[REG_V0] == 10) {
			g_finished = true;
			return;
		}

		handle_exception(core, mem);
		return;
	}

	/* mem_to_reg MUX */
	uint32_t data = MEM_WB.c_mem_to_reg ? MEM_WB.read_data : MEM_WB.alu_res;

	/* Write back */
	if(MEM_WB.c_reg_write) {
		if(GET_OPCODE(MEM_WB.inst) == OPCODE_CP0
		   && GET_RS(MEM_WB.inst) == CP0_MTC0) {
			core->cp0.regs[MEM_WB.reg_dst] = data;
		} else {
			/* Dont write to $0 */
			core->regs[MEM_WB.reg_dst] = MEM_WB.reg_dst == 0 ?
				0 : data;
		}
	}
}

void forwarding_unit(core_t *core)
{
	/* Forwarding is not allowed when:
	 * - MTC0   -> normal instruction
	 * - Normal -> MFC0
	 * In those cases, the register numbers clash with cp0 register numbers.
	 * forwarding to the wrong register!
	 */

	/* SPECIAL CASES */
	/* ERET instruction does not need to be forwarded */
	if(ID_EX.inst == INSTRUCTION_ERET) {
		return;
	}


	/* Forward to A MUX */
	if(EX_MEM.c_reg_write == 1
	   && EX_MEM.reg_dst != 0
	   && EX_MEM.reg_dst == ID_EX.rs) {
		if((GET_OPCODE(EX_MEM.inst) == OPCODE_CP0 && GET_RS(EX_MEM.inst) == CP0_MTC0)
		   && (GET_OPCODE(ID_EX.inst) != OPCODE_CP0))
		{
			DEBUG("ILLEGAL FORWARD CONDITION DETECTED");
			return;
		}
		if((GET_OPCODE(EX_MEM.inst) != OPCODE_CP0) &&
		   (GET_OPCODE(ID_EX.inst) == OPCODE_CP0 && GET_RS(ID_EX.inst) == CP0_MFC0))
		{
			DEBUG("ILLEGAL FORWARD CONDITION DETECTED");
			return;
		}

		DEBUG("FORWARDING 0x%08x from MEM to RS", EX_MEM.alu_res);

		ID_EX.rs_value = EX_MEM.alu_res;

		/* WB */
	} else if(MEM_WB.c_reg_write == 1
		  && MEM_WB.reg_dst != 0
		  && MEM_WB.reg_dst == ID_EX.rs) {
		if((GET_OPCODE(MEM_WB.inst) == OPCODE_CP0 && GET_RS(MEM_WB.inst) == CP0_MTC0)
		   && (GET_OPCODE(ID_EX.inst) != OPCODE_CP0))
		{
			DEBUG("ILLEGAL FORWARD CONDITION DETECTED");

			return;
		}
		if((GET_OPCODE(MEM_WB.inst) != OPCODE_CP0) &&
		   (GET_OPCODE(ID_EX.inst) == OPCODE_CP0 && GET_RS(ID_EX.inst) == CP0_MFC0))
		{
			DEBUG("ILLEGAL FORWARD CONDITION DETECTED");
			return;
		}

		/* WB MUX */
		ID_EX.rs_value = MEM_WB.c_mem_to_reg ?
			MEM_WB.read_data : MEM_WB.alu_res;


		DEBUG("FORWARDING 0x%08x from WB to RS", ID_EX.rs_value);
	}


	/* Forward to B MUX */
	if(EX_MEM.c_reg_write == 1
	   && EX_MEM.reg_dst != 0
	   && EX_MEM.reg_dst == ID_EX.rt) {
		if((GET_OPCODE(EX_MEM.inst) == OPCODE_CP0 && GET_RS(EX_MEM.inst) == CP0_MTC0)
		   && (GET_OPCODE(ID_EX.inst) != OPCODE_CP0))
		{
			DEBUG("ILLEGAL FORWARD CONDITION DETECTED");

			return;
		}
		if((GET_OPCODE(EX_MEM.inst) != OPCODE_CP0) &&
		   (GET_OPCODE(ID_EX.inst) == OPCODE_CP0 && GET_RS(ID_EX.inst) == CP0_MFC0))
		{
			DEBUG("ILLEGAL FORWARD CONDITION DETECTED");

			return;
		}

		DEBUG("FORWARDING 0x%08x from MEM to RT", EX_MEM.alu_res);

		ID_EX.rt_value = EX_MEM.alu_res;

		/* WB */
	} else if(MEM_WB.c_reg_write == 1
		  && MEM_WB.reg_dst != 0
		  && MEM_WB.reg_dst == ID_EX.rt) {
		if((GET_OPCODE(MEM_WB.inst) == OPCODE_CP0 && GET_RS(MEM_WB.inst) == CP0_MTC0)
		   && (GET_OPCODE(ID_EX.inst) != OPCODE_CP0))
		{
			DEBUG("ILLEGAL FORWARD CONDITION DETECTED");

			return;
		}
		if((GET_OPCODE(MEM_WB.inst) != OPCODE_CP0) &&
		   (GET_OPCODE(ID_EX.inst) == OPCODE_CP0 && GET_RS(ID_EX.inst) == CP0_MFC0))
		{
			DEBUG("ILLEGAL FORWARD CONDITION DETECTED");
			return;
		}

		/* WB MUX */
		ID_EX.rt_value = MEM_WB.c_mem_to_reg ?
			MEM_WB.read_data : MEM_WB.alu_res;

		DEBUG("FORWARDING 0x%08x from WB to RT", ID_EX.rt_value);


	}
}
/* Simulates a clock-tick */
void tick(hardware_t *hw)
{
	cpu_t* cpu = hw->cpu;
	mmu_t* mem = hw->mmu;

	/* Iterate over each core */
	int i;
	for(i = 0; i < cpu->num_cores; i++) {
		interpret_wb(cpu->core+i, mem);
		interpret_mem(cpu->core+i, mem);
		interpret_ex(cpu->core+i);
		interpret_id(cpu->core+i);
		interpret_if(cpu->core+i, mem);

		forwarding_unit(cpu->core+i);
	}
}

int run(simulator_t *simulator)
{
	hardware_t *hw = simulator->hw;

	char *buf = (char*)calloc(1, INSTRUCTION_BUFFER_SIZE);
	if(buf == NULL) {
		ERROR("Could not allocate instruction string buffer");
		simulator->logging = false;
	}

	while(g_finished == false && simulator->finished == false) {
		/* XXX: Assumes one core */

		uint32_t inst = 0;
		exception_t e = mmu_read(&hw->cpu->core[0],
					 hw->mmu,
					 hw->cpu->core[0].regs[REG_PC],
					 &inst, MEM_OP_WORD);
		e = e;


		if(simulator->debug) {
			debug(inst, simulator);
		}


		/* Execute the instruction */
		tick(hw);


		if(simulator->logging) {
			/* Instruction address */
			int len = snprintf(buf, INSTRUCTION_BUFFER_SIZE, "0x%08X: ",
					   hw->cpu->core[0].regs[REG_PC]);

			fputs(buf, simulator->log_fh);
			memset(buf, 0, len);

			len = instruction_string(inst,
						 &hw->cpu->core[0],
						 buf,
						 INSTRUCTION_BUFFER_SIZE);

			fputs(buf, simulator->log_fh);

			memset(buf, 0, INSTRUCTION_BUFFER_SIZE);
		}

	}

	free(buf);

	/* XXX */
	LOG("RESULT: %d", hw->cpu->core[0].regs[REG_V1]);
	return hw->cpu->core[0].regs[REG_V1];
}

static void init_io(hardware_t *hw)
{
	LOG();
	mmu_add_device(hw->mmu, shutdown_device_create());
}


static hardware_t* hw_init(size_t cores, size_t memsz)
{
	hardware_t *hw = malloc(sizeof(struct hardware));

	if(hw == NULL) {
		ERROR("Could not allocate hw.");
		exit(1);
	}

	/* Initialize the memory */
	hw->mmu = mmu_init(memsz);
	if(hw->mmu == NULL) {
		ERROR("Failed to init MMU");
		exit(-1);
	}


	/* Create a new CPU */
	hw->cpu = cpu_init(cores);
	if(hw->cpu == NULL) {
		ERROR("Failed to init CPU");
		exit(-1);
	}


	/* Set stack pointer to top of memory */
	/* XXX: The OS should do this by itself, but for testing programs, we
	 * will let it be.*/
	hw->cpu->core[0].regs[REG_SP] = (uint32_t)(KSEG0_VSTART +
						   hw->mmu->size_kseg0) - 4;

	DEBUG("Stack-Pointer set to: 0x%08x", hw->cpu->core[0].regs[REG_SP]);

	/* Initialize basic devices */
	init_io(hw);

	return hw;
}

void hw_free(hardware_t *hw)
{
	/* Free allocated resources */
	cpu_free(hw->cpu);
	mmu_free(hw->mmu);
	free(hw);
}


int simulate(simulator_t *simulator)
{
	/* Initialize hardware */
	hardware_t *hw = hw_init(simulator->cores, simulator->memsz);
	simulator->hw = hw;

	/* Load the program into memory */
	int retval;
	if((retval = elf_dump(simulator->program,
			      &(hw->cpu->core[0].regs[REG_PC]),
			      hw->mmu)) != 0) {
		ERROR("Elf file could not be read: %d.", retval);
		exit(0);
	}

	int ret = run(simulator);

	return ret;
}
