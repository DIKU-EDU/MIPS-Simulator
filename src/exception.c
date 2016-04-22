
#include "cpu.h"
#include "exception.h"

void raise_exception(uint32_t exception_code, core_t *core)
{
	return;
}

/* Array of register names */
char *exc_names [] = {
	"None",
	"Interrupt",
	"AddressErrorLoad",
	"AddressErrorStore",
	"BusErrorInstructionFetch",
	"BusErrorLoadStore",
	"Syscall",
	"Breakpoint",
	"Reserved",
	"CpUnavailable",
	"ArithmeticOverflow",
	"Trap",
	"FloatingPoint",
	NULL
};
