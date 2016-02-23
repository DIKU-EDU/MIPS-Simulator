#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "cpu.h"

/*
 * Core structure of the CPU
 */
typedef struct core {
	uint32_t regs[32];
} core_t;


