#ifndef _DEBUG_H
#define _DEBUG_H

#include "stdint.h"
#include "mem.h"
#include "cpu.h"


/* Reads STDIN and prints relevant debugging info */
void debug(uint32_t inst, core_t *core, memory_t *mem);

/* Prints all registers of a given core. */
void print_registers(core_t *core);

/* Dump registers except PC in hex. Used for testing */
void dump_registers(core_t *core);

/* Print pipeline registers in columns */
void print_pipeline_registers(core_t *core);


#endif /* _DEBUG_H */
