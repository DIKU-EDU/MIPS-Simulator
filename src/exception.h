#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include <stdint.h>
#include "cpu.h"

#define NUM_EXCEPTIONS 16
extern char *exc_names[NUM_EXCEPTIONS];	/* Defined in exception.c */

/* Returns the value the CP0.Cause register should have before jumping to
 * exception handler code.
 * in_branch_delay flags if exception occured in an branch delay slot */
uint32_t get_cause(exception_t exception, bool in_branch_delay);

#endif /* _EXCEPTION_H */
