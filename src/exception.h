#ifndef _EXCEPTION_H
#define _EXCEPTION_H


/* Exception codes
 * COD Green Card */
typedef enum exceptions {
	EXC_None,
	EXC_Interrupt,
	EXC_AddressErrorLoad,
	EXC_AddressErrorStore,
	EXC_BusErrorInstructionFetch,
	EXC_BusErrorLoadStore,
	EXC_Syscall,
	EXC_Breakpoint,
	EXC_Reserved,
	EXC_CpUnavailable,
	EXC_ArithmeticOverflow,
	EXC_Trap,
	EXC_FloatingPoint
} exception_t;


void raise_exception(uint32_t exception_code, core_t *core);

#endif /* _EXCEPTION_H */
