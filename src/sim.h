#ifndef _SIM_H
#define _SIM_H

#include <stdio.h>
#include <stdint.h>

#include "cpu.h"
#include "mem.h"
#include "io.h"

typedef struct hardware {
	cpu_t* cpu;
	mmu_t *mmu;
} hardware_t;

/* Collection of important simulator-related variables */
typedef struct simulator {
	/* Simulator cores */
	size_t cores;

	/* Allocated memory */
	size_t memsz;

	/* filename of the program being run */
	char *program;

	/* Logging */
	bool logging;
	FILE *log_fh;
	char *log_file;

	/* Indicates if simulator is finished */
	bool finished;

	/* Indicates whether debugging is enabled */
	bool debugging;

	/* Pointer to the hardware */
	hardware_t *hw;
} simulator_t;


int simulate(simulator_t *simulator);

/* Deallocates hardware component as well as the subsequent structures */
void sim_free(hardware_t *hw);

#endif /* _SIM_H */
