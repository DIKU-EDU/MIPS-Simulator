#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "cpu.h"
#include "error.h"


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

	if(cores == NULL || cpu == NULL) {
		ERROR("Cannot allocate cpu structure(s).");
		return NULL;
	}

	cpu->core = cores;
	cpu->num_cores = n_cores;


	/* Initialize core */
	/* Enable Global Interrupt Bit SR(IE) */
	cpu->core[0].cp0.regs[REG_SR] |= SR_IE;


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

