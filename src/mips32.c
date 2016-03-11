#include "mips32.h"

char* op_codes[] = {
	"SPECIAL",
	"REGIMM",
	"J",
	"JAL",
	"BEQ",
	"BNE",
	"BLEZ",
	"BGTZ",
	"ADDI",
	"ADDIU",
	"SLTI",
	"SLTIU",
	"ANDI",
	"ORI",
	"XORI",
	"LUI",
	"COP0",
	"COP1",
	"COP2",
	"COP3",
	"BEQL",
	"BNEL",
	"BLEZL",
	"BGTZL",
	"Invalid instruction",
	"Invalid instruction",
	"Invalid instruction",
	"Invalid instruction",
	"SPECIAL2",
	"Invalid instruction",
	"Invalid instruction",
	"Invalid instruction",
	"LB",
	"LH",
	"LWL",
	"LW",
	"LBU",
	"LHU",
	"LWR",
	"Invalid instruction",
	"SB",
	"SH",
	"SWL",
	"SW",
	"Invalid instruction",
	"Invalid instruction",
	"SWR",
	"CACHE",
	"LL",
	"LWC1",
	"LWC2",
	"PREF",
	"Invalid instruction",
	"LDC1",
	"LDC2",
	"Invalid instruction",
	"SC",
	"SWC1",
	"SWC2",
	"Invalid instruction",
	"Invalid instruction",
	"SDC1",
	"SDC2",
	"Invalid instruction",
	NULL
};

char *funct_codes[] = {
	"SLL",
	"MOVC1",
	"SRL",
	"SRA",
	"SLLV",
	"Invalid function",
	"SRLV",
	"SRAV",
	"JR",
	"JALR",
	"MOVZ",
	"MOVN",
	"SYSCALL",
	"BREAK",
	"Invalid function",
	"SYNC",
	"MFHI",
	"MTHI",
	"MFLO",
	"MTLO",
	"Invalid function",
	"Invalid function",
	"Invalid function",
	"Invalid function",
	"MULT",
	"MULTU",
	"DIV",
	"DIVU",
	"Invalid function",
	"Invalid function",
	"Invalid function",
	"Invalid function",
	"ADD",
	"ADDU",
	"SUB",
	"SUBU",
	"AND",
	"OR",
	"XOR",
	"NOR",
	"Invalid function",
	"Invalid function",
	"SLT",
	"SLTU",
	"Invalid function",
	"Invalid function",
	"Invalid function",
	"Invalid function",
	"TGE",
	"TGEU",
	"TLT",
	"TLTU",
	"TEQ",
	"Invalid function",
	"TNE",
	"Invalid function",
	"Invalid function",
	"Invalid function",
	"Invalid function",
	"Invalid function",
	"Invalid function",
	"Invalid function",
	"Invalid function",
	"Invalid function",
	NULL
};

