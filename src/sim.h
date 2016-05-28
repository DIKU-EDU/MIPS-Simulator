#ifndef _SIM_H
#define _SIM_H

#include <stdio.h>
#include <stdint.h>

#include "cpu.h"
#include "mem.h"
#include "io.h"

/* Collection of important simulator-related variables */
typedef struct simulator {
	/* Simulator cores */
	size_t cores;

	/* Allocated memory */
	size_t memory;

	/* filename of the program being run */
	char *filename;

	/* Logging */
	bool logging;
	FILE *fh_log;

	/* Indicates if simulator is finished */
	bool finished;

	/* Indicates whether debugging is enabled */
	bool debugging;
} simulator_t;

typedef struct hardware {
	cpu_t* cpu;
	mmu_t *mmu;
} hardware_t;

int simulate(char *program, size_t cores, size_t mem, bool debug, bool log);

/* Deallocates hardware component as well as the subsequent structures */
void sim_free(hardware_t *hw);

#endif /* _SIM_H */
