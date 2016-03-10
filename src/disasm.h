#ifndef _DISASM_H
#define _DISASM_H

#include <stdint.h>

#include "cpu.h"

/* Prints the */
void print_instruction(uint32_t instr, core_t *core);

#endif /* _DISASM_H */
