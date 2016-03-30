#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "cpu.h"

/* Array of register names */
char *reg_names [] = {
	"zero",
	"at",
	"v0",
	"v1",
	"a0",
	"a1",
	"a2",
	"a3",
	"t0",
	"t1",
	"t2",
	"t3",
	"t4",
	"t5",
	"t6",
	"t7",
	"s0",
	"s1",
	"s2",
	"s3",
	"s4",
	"s5",
	"s6",
	"s7",
	"t8",
	"t9",
	"k0",
	"k1",
	"gp",
	"sp",
	"fp",
	"ra",
	"pc",
};

cpu_t*
cpu_init(int n_cores)
{
	core_t *cores = (core_t*)calloc(n_cores, sizeof(core_t));
	cpu_t *cpu = (cpu_t*)calloc(1, sizeof(cpu_t));

	cpu->core = cores;
	cpu->num_cores = n_cores;

	return cpu;
}

void
cpu_free(cpu_t *cpu)
{
	free(cpu->core);
	free(cpu);
}

void
cpu_tick(cpu_t *cpu)
{
	/* Do stuff */
}



/*
 * Looks up register name and returns its number.
 * Returns -1 on error. */
uint32_t
register_to_number(char *str)
{
	size_t i;
	for(i = 0; i < NUM_REGISTERS; i++) {
		/* If strings equal */
		if(strcmp(str,reg_names[i]) == 0) {
			return i;
		}
	}
	return 0;
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
	printf("---------------------------------------------------------------------------------------------\n");
	printf("IF/ID              ID/EX                      EX/MEM                      MEM/WB\n");
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

