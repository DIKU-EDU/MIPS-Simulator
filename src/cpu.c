#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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
int
register_to_number(char *str)
{
	size_t i;
	for(i = 0; i < NUM_REGISTERS; i++) {
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




