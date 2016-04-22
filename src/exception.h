#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include "cpu.h"

#define NUM_EXCEPTIONS 16
extern char *exc_names[NUM_EXCEPTIONS];	/* Defined in exception.c */

void raise_exception(uint32_t exception_code, core_t *core);

#endif /* _EXCEPTION_H */
