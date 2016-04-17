#ifndef _MEM_H
#define _MEM_H

#include "cpu.h"
#include "exception.h"

/* Sizes of virtual memory segments */
#define KSEG2_SIZE 0x400000000
#define KSEG1_SIZE 0x200000000
#define KSEG0_SIZE 0x200000000
#define KUSEG_SIZE 0x800000000

/* Start of virtual memory segments */
#define KSEG2_VSTART 0xC0000000
#define KSEG1_VSTART 0xA0000000
#define KSEG0_VSTART 0x80000000
#define KUSEG_VSTART 0x00000000

/* Start of physical memory segments */
#define KSEG1_PSTART 0x00000000
#define KSEG0_PSTART 0x20000000
#define KUSEG_PSTART (KSEG1_PSTART + KSEG0_PSTART)
#define KSEG2_PSTART (KUSEG_PSTART + KUSEG_SIZE)


/* Memory operations size */
typedef enum {
	MEM_OP_BYTE,
	MEM_OP_HALF,
	MEM_OP_WORD
} mem_op_size_t; /* Memory Operation Size*/



typedef struct memory {
	uint8_t *pmem;	/* Raw memory */
	uint32_t size_total;  /* Allocated memory */

	/* Sizes of the different segments.
	 * Sum up to size_total */
	uint32_t size_kseg0;
	uint32_t size_kseg1;
	uint32_t size_kseg2;
	uint32_t size_kuseg;
} memory_t;

/* Reads from memory and writes to dst. Returns a relevant exception */
exception_t mem_read(core_t *core, memory_t *mem, int32_t vaddr, uint32_t *dst,
		     mem_op_size_t op_size);

/* Writes src to addr. Returns a relevant exception. */

exception_t mem_write(core_t *core, memory_t *mem, int32_t addr, uint32_t src,
		      mem_op_size_t op_size);

/* Translate virtual memory address to physical */
uint32_t addr_translate(uint32_t vaddr);

/* Returns a new memory block */
memory_t* mem_init(size_t size);

/* Frees ressources used by mem */
void mem_free(memory_t *mem);


#endif /* _MEM_H */
