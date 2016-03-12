#ifndef _SIM_H
#define _SIM_H

#include "cpu.h"
#include "mem.h"

typedef struct hardware {
	cpu_t* cpu;
	memory_t *mem;
} hardware_t;

int simulate(char *program, bool debug);

#endif /* _SIM_H */
