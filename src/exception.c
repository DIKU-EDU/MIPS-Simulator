#include <stdint.h>

#include "cpu.h"
#include "exception.h"
#include "cp0.h"

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

uint32_t get_cause(exception_t exception, bool in_branch_delay)
{
	uint32_t cause = 0;

	/* Exception happened in branch_delay, set BD (bit 31) */
	if(in_branch_delay) {
		cause |= CAUSE_BD;
	}

	switch(exception) {
	case EXC_None:
		return 0;
	case EXC_Interrupt:
		cause |= CAUSE_ExcCode_Interrupt;
		break;
	case EXC_AddressErrorLoad:
		cause |= CAUSE_ExcCode_AdEL;
		break;

	case EXC_AddressErrorStore:
		cause |= CAUSE_ExcCode_AdES;
		break;

	case EXC_BusErrorInstructionFetch:
		cause |= CAUSE_ExcCode_IBE;
		break;

	case EXC_BusErrorLoadStore:
		cause |= CAUSE_ExcCode_DBE;
		break;

	case EXC_Syscall:
		cause |= CAUSE_ExcCode_Syscall;
		break;

	case EXC_Breakpoint:
		cause |= CAUSE_ExcCode_Bp;
		break;

	case EXC_Reserved:
		cause |= CAUSE_ExcCode_RI;
		break;

	case EXC_CpUnavailable:
		cause |= CAUSE_ExcCode_CpU;
		break;

	case EXC_ArithmeticOverflow:
		cause |= CAUSE_ExcCode_Ov;
		break;

	case EXC_Trap:
		cause |= CAUSE_ExcCode_TRAP;
		break;

	case EXC_FloatingPoint:
		cause |= CAUSE_ExcCode_FPE;
		break;

	}

	return cause;
}
