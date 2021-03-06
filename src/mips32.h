// Copyright (c) 2016 DIKU.
// Copenhagen Liberal License - v0.6 - January 25, 2016.
//
// Authors (most recent contributors first):
//  Oleksandr Shturmov <oleks@oleks.info>
//  Annie Jane Pinder <anpi@di.ku.dk>

#ifndef _MIPS32_H
#define _MIPS32_H


#include <stdint.h>
#include <stdlib.h>

#if 0
// MIPS32 memory
#define GET_BIGWORD(mem, addr) ((uint32_t) \
  ((mem)[(addr) + 0] << 24)  | \
  ((mem)[(addr) + 1] << 16)  | \
  ((mem)[(addr) + 2] << 8)   | \
  ((mem)[(addr) + 3]))

#define GET_BIGBYTE(mem, addr) ((uint32_t)(uint8_t) \
  ((mem)[(addr)]))

#define GET_BIGHALF(mem, addr) ((uint32_t)(uint16_t) \
  ((mem)[(addr) + 0] << 8) | \
  ((mem)[(addr) + 1]))
#endif


/* Dumps the next n bytes to stdout */
void dump_mem(uint8_t* mem, uint32_t addr, uint32_t n);

#define DUMP_MEM(mem, addr, n) do { \
	dump_mem(mem, addr - MIPS_RESERVE, n);\
	} while(0)


// MIPS32 instruction parsing
/* OPCODES */
#define OPCODE_R        (0x00)
#define OPCODE_J        (0x02)
#define OPCODE_JAL      (0x03)
#define OPCODE_BEQ      (0x04)
#define OPCODE_BNE      (0x05)
#define OPCODE_ADDI     (0x08)
#define OPCODE_ADDIU    (0x09)
#define OPCODE_SLTI     (0x0A)
#define OPCODE_SLTIU    (0x0B)
#define OPCODE_ANDI     (0x0C)
#define OPCODE_ORI      (0x0D)
#define OPCODE_LUI      (0x0F)
#define OPCODE_LWL	(0x22)
#define OPCODE_LW       (0x23)
#define OPCODE_LBU	(0x24)
#define OPCODE_LHU	(0x25)
#define OPCODE_LWR	(0x26)
#define OPCODE_LL	(0x30)
#define OPCODE_SW       (0x2B)
#define OPCODE_SB	(0x28)
#define OPCODE_SC	(0x38)
#define OPCODE_SH	(0x29)
#define OPCODE_SWR	(0x2A)
#define OPCODE_SWL	(0x2E)

/* Special OPCODES */
#define OPCODE_CP0	(0x10)

/* FUNCTS */
#define FUNCT_JR        (0x08)
#define FUNCT_SYSCALL   (0x0C)
#define FUNCT_ADD       (0x20)
#define FUNCT_ADDU      (0x21)
#define FUNCT_SUB       (0x22)
#define FUNCT_SUBU      (0x23)
#define FUNCT_AND       (0x24)
#define FUNCT_OR        (0x25)
#define FUNCT_NOR       (0x27)
#define FUNCT_SLT       (0x2A)
#define FUNCT_SLTU      (0x2B)
#define FUNCT_SLL       (0x00)
#define FUNCT_SRL       (0x02)

/* Special RS-field encodings */
#define CP0_MFC0 0x00
#define CP0_MTC0 0x04

/* Other instructions */
#define INSTRUCTION_ERET (0x42000018)


extern char* cp0_codes[];
extern char* op_codes[];
extern char* funct_codes[];

// Least-significant bit-masks.
#define LS_4B   ((1 << 4 ) - 1)
#define LS_5B   ((1 << 5 ) - 1)
#define LS_6B   ((1 << 6 ) - 1)
#define LS_8B	((1 << 8 ) - 1)
#define LS_16B  ((1 << 16) - 1)
#define LS_26B  ((1 << 26) - 1)

// Most-significant bit-masks.
#define MS_4B   (LS_4B << 28)

#define GET_OPCODE(inst)    ( (inst) >> 26)
#define GET_RS(inst)        (((inst) >> 21) & LS_5B)
#define GET_RT(inst)        (((inst) >> 16) & LS_5B)
#define GET_RD(inst)        (((inst) >> 11) & LS_5B)
#define GET_SHAMT(inst)     (((inst) >> 6)  & LS_5B)

#define GET_FUNCT(inst)     ((inst) & LS_6B)
#define GET_IMM(inst)       ((inst) & LS_16B)
#define GET_ADDRESS(inst)   ((inst) & LS_26B)

#define SIGN_EXTEND(value)  ((int32_t)(int16_t)value)
#define ZERO_EXTEND(value)  ((uint32_t)value)
#define ZERO_EXTEND_MASK	(uint32_t)0x0000FFFF

/* Exception codes
 * COD Green Card */
typedef enum exceptions {
	EXC_None = 0,
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

#endif // MIPS32_H
