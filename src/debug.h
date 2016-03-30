#ifndef _DEBUG_H
#define _DEBUG_H

#include "stdint.h"

#include "cpu.h"

/* Reads STDIN and prints relevant debugging info */
void debug(uint32_t inst, core_t *core);

#endif /* _DEBUG_H */
