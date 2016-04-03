#include <stdio.h>

#include "debug.h"
#include "cpu.h"
#include "disasm.h"
#include "error.h"

/* extern'd in sim.c */
extern bool g_debugging;

/* extern'd in sim.c */
extern bool g_finished;

void debug(uint32_t inst, core_t* core)
{
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
p		- Prints pipeline registers\n\
i		- Prints current instruction\n\
[v|s|t]<N>	- Prints register v/s/t number <N>\n\
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

		case 'p':
			print_pipeline_registers(core);
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
	printf("%s                %s                        %s                         %s                        \n",
	       INST_STR(IF_ID.inst), INST_STR(ID_EX.inst),INST_STR(EX_MEM.inst),
	       INST_STR(MEM_WB.inst));

	printf("---------------------------------------------------------------------------------------------\n");
	printf("inst:    %08x  c_reg_dst:   %08x      c_reg_write: %08x      c_reg_write:  %08x\n",
	      IF_ID.inst, ID_EX.c_reg_dst, EX_MEM.c_reg_write, MEM_WB.c_reg_write);
	printf("next_pc: %08x  c_alu_op:    %08x      c_branch:    %08x      c_mem_to_reg: %08x\n",
	       IF_ID.next_pc, ID_EX.c_alu_op, EX_MEM.c_branch, MEM_WB.c_mem_to_reg);
	printf("                   c_alu_src:   %08x      c_mem_read:  %08x      alu_res:      %8x\n",
	       ID_EX.c_alu_src, EX_MEM.c_mem_read, MEM_WB.alu_res);
	printf("                   c_branch:    %08x      c_mem_write: %08x      read_data:    %8x\n",
	       ID_EX.c_branch, EX_MEM.c_mem_write, MEM_WB.read_data);
	printf("                   c_mem_read:  %08x      c_mem_to_reg:%08x      reg_dst:      %8d\n",
	       ID_EX.c_mem_read, EX_MEM.c_mem_to_reg, MEM_WB.reg_dst);
	printf("                   c_mem_write: %08x      branch_target%08x\n",
	       ID_EX.c_mem_write, EX_MEM.branch_target);
	printf("                   c_reg_write: %08x      eff_addr:    %08x\n",
	       ID_EX.c_reg_write, EX_MEM.eff_addr);
	printf("                   c_mem_to_reg:%08x      alu_res:     %08x\n",
	       ID_EX.c_mem_to_reg, EX_MEM.alu_res);
	printf("                   shamt:       %08x      rt_value:    %08x\n",
	       ID_EX.shamt, EX_MEM.rt_value);
	printf("                   funct:       %08x      reg_dst:     %08x \n",
	       ID_EX.funct, EX_MEM.reg_dst);
	printf("                   next_pc:     %8x\n",
	       ID_EX.next_pc);
	printf("                   rs_value:    %8x\n",
	       ID_EX.rs_value);
	printf("                   rt_value:    %8x\n",
	       ID_EX.rt_value);
	printf("                   sign_ext_imm:%8x\n",
	       ID_EX.sign_ext_imm);
	printf("                   rs:          %8x\n",
	       ID_EX.rs);
	printf("                   rt:          %8x\n",
	       ID_EX.rt);
	printf("                   rd:          %8x\n",
	       ID_EX.rd);

	printf("\n\n");
}

