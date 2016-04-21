#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include "cpu.h"

void raise_exception(uint32_t exception_code, core_t *core);

#endif /* _EXCEPTION_H */
