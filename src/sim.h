#ifndef _SIM_H
#define _SIM_H

#include "cpu.h"
#include "mem.h"
#include "io.h"


typedef struct hardware {
	cpu_t* cpu;
	mmu_t *mmu;

} hardware_t;

int simulate(char *program, size_t cores, size_t mem, bool debug);

/* Deallocates hardware component as well as the subsequent structures */
void sim_free(hardware_t *hw);

#endif /* _SIM_H */
