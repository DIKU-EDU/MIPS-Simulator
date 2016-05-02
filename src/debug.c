#include <stdio.h>

#include "debug.h"
#include "cpu.h"
#include "disasm.h"
#include "error.h"
#include "mem.h"
#include "exception.h"

/* extern'd in sim.c */
extern bool g_debugging;

/* extern'd in sim.c */
extern bool g_finished;

void debug(uint32_t inst, core_t* core, memory_t *mem)
{
	printf("PC: 0x%08X\n", core->regs[REG_PC]);
	print_instruction(inst, core);

	unsigned char c[3] = {0};
	bool stop = false;

	while(stop == false) {
		printf("> ");
		fgets((char*)c, 3, stdin);

		switch(c[0]) {
		case 'h':
			printf("DEBUG COMMANDS:\n\
h		- Prints this message\n\
r		- Prints all GP registers\n\
0		- Prints all special registers (CP0)\n\
p		- Prints pipeline registers\n\
i		- Prints current instruction\n\
[v|s|t]<N>	- Prints register v/s/t number <N>\n\
m		- Print from memory (will ask for addr)\n\
c		- Continue executing\n\
q		- Quit\n\
'\\n'		- Step\n\n");
			break;

			/* Print all registers */
		case 'r':
			print_registers(core);
			break;

			/* Print instruction */
		case 'i':
			print_instruction(inst, core);
			break;

		/* inspect memory */
		case 'm':
			printf("@ 0x");
			uint32_t addr = 0;
			uint32_t word = 0;
			if(scanf("%X", &addr) > 0) {
				mem_read(core, mem, addr, &word, MEM_OP_WORD);
			}
			fseek(stdin,0,SEEK_END);

			printf("[0x%08X] = 0x%08X\n", addr, word);
			break;
		case 'p':
			print_pipeline_registers(core);
			break;
			/* Print specified register only */

		case '0':
			cp0_dump_registers(&core->cp0);

		case 'v':
		case 's':
		case 't':
			printf("%s = %u\n", c,
			       core->regs[register_to_number((char*)c)]);
			break;

			/* Continue */
		case 'c':
			/* XXX: This is ugly */
			g_debugging = false;
			break;

		/* Exit */
		case 'q':
			g_finished = true;
			return;
		case '\n':
		default:
			return;
		}
	}
	printf("\n");
}


void
print_registers(core_t *core)
{
	int i;
	for(i = 0; i < NUM_REGISTERS; i++) {
		printf("%s\t(s) %d\t(u) %u\t 0x%x\n",reg_names[i],
		       (int32_t)core->regs[i], (uint32_t)core->regs[i],
		       (uint32_t)core->regs[i]);
	}
}

void
dump_registers(core_t *core)
{
	size_t i;
	/* PC is not included */
	for(i = 0; i < NUM_REGISTERS - 1; i++) {
		printf("%d\n", core->regs[i]);
	}
}

void print_pipeline_registers(core_t *core)
{
	/* MACROS for less typing */
#define IF_ID (core->if_id)
#define ID_EX (core->id_ex)
#define EX_MEM (core->ex_mem)
#define MEM_WB (core->mem_wb)
#define PC (core->regs[REG_PC])
#define INST_STR(inst) GET_OPCODE((inst)) == 0 ? funct_codes[GET_FUNCT((inst))]\
						: op_codes[GET_OPCODE((inst))]

	printf("---------------------------------------------------------------------------------------------\n");
	printf("IF/ID              ID/EX                      EX/MEM                      MEM/WB\n");
	printf("%s                  %s                        %s                         %s                        \n",
	       INST_STR(IF_ID.inst), INST_STR(ID_EX.inst),INST_STR(EX_MEM.inst),
	       INST_STR(MEM_WB.inst));

	printf("---------------------------------------------------------------------------------------------\n");
	printf("next_pc:    %08x  next_pc:   %08x      next_pc: %08x      next_pc:  %08x\n",
	      IF_ID.next_pc, ID_EX.next_pc, EX_MEM.next_pc, MEM_WB.next_pc);

	printf("inst:    %08x  c_reg_dst:   %08x      c_reg_write: %08x      c_reg_write:  %08x\n",
	      IF_ID.inst, ID_EX.c_reg_dst, EX_MEM.c_reg_write, MEM_WB.c_reg_write);
	printf("next_pc: %08x  c_alu_op:    %08x      c_branch:    %08x      c_mem_to_reg: %08x\n",
	       IF_ID.next_pc, ID_EX.c_alu_op, EX_MEM.c_branch, MEM_WB.c_mem_to_reg);
	printf("                   c_alu_src:   %08x      c_mem_read:  %08x      alu_res:      %8x\n",
	       ID_EX.c_alu_src, EX_MEM.c_mem_read, MEM_WB.alu_res);
	printf("                   c_beq:       %08x      c_mem_write: %08x      read_data:    %8x\n",
	       ID_EX.c_beq, EX_MEM.c_mem_write, MEM_WB.read_data);
	printf("                   c_mem_read:  %08x      c_mem_to_reg:%08x      reg_dst:      %8d\n",
	       ID_EX.c_mem_read, EX_MEM.c_mem_to_reg, MEM_WB.reg_dst);
	printf("                   c_mem_write: %08x      \n",
	       ID_EX.c_mem_write);
	printf("                   c_reg_write: %08x      eff_addr:    %08x\n",
	       ID_EX.c_reg_write, EX_MEM.eff_addr);
	printf("                   c_mem_to_reg:%08x      alu_res:     %08x\n",
	       ID_EX.c_mem_to_reg, EX_MEM.alu_res);
	printf("                   c_jump:      %08x      rt_value:    %08x\n",
		ID_EX.c_jump, EX_MEM.rt_value);
	printf("                   shamt:       %08x      reg_dst:     %08x\n",
	       ID_EX.shamt, EX_MEM.reg_dst);
	printf("                   c_bne:       %08x\n",
		ID_EX.c_bne);
	printf("                   funct:       %08x\n",
	       ID_EX.funct);
	printf("                   next_pc:     %8x\n",
	       ID_EX.next_pc);
	printf("                   rs_value:    %8x\n",
	       ID_EX.rs_value);
	printf("                   rt_value:    %8x\n",
	       ID_EX.rt_value);
	printf("                   jump_addr:   %8x\n",
	       ID_EX.jump_addr);
	printf("                   sign_ext_imm:%8x\n",
	       ID_EX.sign_ext_imm);
	printf("                   rs:          %8x\n",
	       ID_EX.rs);
	printf("                   rt:          %8x\n",
	       ID_EX.rt);
	printf("                   rd:          %8x\n",
	       ID_EX.rd);
	printf("EXC:\t%6s\t\tEXC:\t%6s\t\tEXC:\t%6s\t\tEXC:\t%6s\n",
	      exc_names[IF_ID.exception],
	       exc_names[ID_EX.exception],
	       exc_names[EX_MEM.exception],
	       exc_names[MEM_WB.exception]);
	printf("is_branch_delay: %u\t\t\t%u\t\t\t%u\t\t\t%u\n",
	     IF_ID.is_branch_delay,
	     ID_EX.is_branch_delay,
	     EX_MEM.is_branch_delay,
	     MEM_WB.is_branch_delay);
	printf("BadVAddr: %08x\t\t%08x\t\t%08x\t\t%08x\n",
	     IF_ID.BadVAddr,
	     ID_EX.BadVAddr,
	     EX_MEM.BadVAddr,
	     MEM_WB.BadVAddr);

	printf("\n\n");
}

