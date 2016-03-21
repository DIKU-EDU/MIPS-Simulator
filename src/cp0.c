#include <stdlib.h>
#include "cp0.h"

const char *cp0_reg_names[] = {
	"Context",	/* memory management (TLB) */
	"Random",
	"EntryLo0",
	"EntryLo1",
	"Context",
	"PageMask",
	"Wired",
	"EntryHi",
	"HWREna",	/* Sets user-privilege programs permissions */
	"BadVAddr",	/* Program address of the violation */
	"Count",	/* high-resolution time */
	"Compare",
	"SR",		/* Status Register */
	"IntCtl",	/* Interrupt vector setup */
	"SRSCtl",	/* Shadow register control */
	"SRSMap",	/* Shadow register map */
	"Cause",	/* Cause Register */
	"EPC",		/* Exception Program Counter */
	"PRId",		/* Product ID register */
	"EBase",	/* Exception entry point base address */
	"Config",	/* CPU setup */
	"Config1",
	"Config2",
	"Config3",
	"LLAddr",	/* Cache address */
	"Debug",	/* EJTAG debug */
	"DEPC",
	"DESAVE",
	"CacheErr",	/* Memory error analysis registers */
	"ECC",
	"ErrorEPC",
	"TagLo",	/* Cache manipulation */
	"DataLo",
	"TagHi",
	"DataHi",
	"WatchLo",	/* Data watchpoint facility */
	"WatchHi",
	"PerfCtl",
	"PerfCnt",
	NULL
};
