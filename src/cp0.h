#ifndef _CP0_H
#define _CP0_H
#include <stdint.h>

const char *cp0_reg_names[];

#define CP0_NUM_REGS	39

#define	REG_CONTEXT	0
#define	REG_RANDOM	1
#define	REG_ENTRYLO0	2
#define	REG_ENTRYLO1	3
#define	REG_CONTEXT	4
#define	REG_PAGEMASK	5
#define	REG_WIRED	6
#define	REG_ENTRYHI	7
#define	REG_HWRENA	8
#define	REG_BADVADDR	9
#define	REG_COUNT	10
#define	REG_COMPARE	11
#define	REG_SR		12
#define	REG_INTCTL	13
#define	REG_SRSCTL	14
#define	REG_SRSMAP	15
#define	REG_CAUSE	16
#define	REG_EPC		17
#define	REG_PRID	18
#define	REG_EBASE	19
#define	REG_CONFIG	20
#define	REG_CONFIG1	21
#define	REG_CONFIG2	22
#define	REG_CONFIG3	23
#define	REG_LLADDR	24
#define	REG_DEBUG	25
#define	REG_DEPC	26
#define	REG_DESAVE	27
#define	REG_CACHEERR	28
#define	REG_ECC		29
#define	REG_ERROREPC	30
#define	REG_TAGLO	31
#define	REG_DATALO	32
#define	REG_TAGHI	33
#define	REG_DATAHI	34
#define	REG_WATCHLO	35
#define	REG_WATCHHI	36
#define	REG_PERFCTL	37
#define	REG_PERFCNT	38


typedef struct cp0 {
	uint32_t regs[CP0_NUM_REGS];

} cp0_t;

#endif /* _CP0_H */
