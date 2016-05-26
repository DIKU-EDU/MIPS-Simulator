#ifndef _DISASM_H
#define _DISASM_H

#include <stdint.h>

#include "cpu.h"


#define INSTRUCTION_BUFFER_SIZE 1024

/* Prints the instruction to STDOUT */
void print_instruction(uint32_t instr, core_t *core);

/* Writes the instruction to a buffeer.
 * RETURNS: number of bytes written
 *
 * TODO: Buffer overflow here? */
int instruction_string(uint32_t instr, core_t *core, char *buf, size_t buf_size);

#endif /* _DISASM_H */
