#ifndef _MEM_H
#define _MEM_H

#include "mem.h"
#include "cpu.h"
#include "exception.h"
#include "io.h"

/* Sizes of virtual memory segments */
#define KSEG2_SIZE 0x40000000
#define KSEG1_SIZE 0x20000000
#define KSEG0_SIZE 0x20000000
#define KUSEG_SIZE 0x80000000

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

/* TODO: Addresses 0x00000000 through 0xfffeffff are used for memory, and
 * addresses 0xffff0000 - 0xffffffff (the last 64 kilobytes) are reserved for
 * I/O device registers */
#define IO_REGISTER_AREA		0xFFFF0000
#define IO_DESCRIPTOR_AREA		0xFFFE0000
#define IO_DESCRIPTOR_AREA_LENGTH	0x00010000



/* Number of IO devices */
#define MAX_IO_DEVICES		(IO_DESCRIPTOR_AREA_LENGTH / sizeof(struct device_descriptor))

/* Macros for reading from memory */
#define GET_BIGWORD(addr) ((uint32_t) \
			   ((addr)[0] << 24)  | \
			   ((addr)[1] << 16)  | \
			   ((addr)[2] << 8)   | \
			   ((addr)[3]))

#define GET_BIGBYTE(addr) ((uint32_t)(uint8_t) \
			   ((addr)[0]))

#define GET_BIGHALF(addr) ((uint32_t)(uint16_t) \
			   ((addr)[0] << 8) | \
			   ((addr)[1]))


#define SET_BIGWORD(addr, value) \
	(addr)[0] = ((value) >> 24); \
(addr)[1] = ((value) << 8 >> 24); \
(addr)[2] = ((value) << 16 >> 24); \
(addr)[3] = ((value) << 24 >> 24);

#define SET_BIGBYTE(addr, value) \
	(addr)[0] = ((value));

#define SET_BIGHALF(addr, value) \
	(addr)[0] = (value) >> 8; \
(addr)[1] = (value) << 24 >> 24;

/* Memory operations size */
typedef enum {
	MEM_OP_BYTE,
	MEM_OP_HALF,
	MEM_OP_WORD
} mem_op_size_t; /* Memory Operation Size */



typedef struct mmu {
	uint8_t *pmem;	/* Raw memory */
	uint32_t size_total;  /* Allocated memory */

	/* Pointer to IO descriptor area */
	device_descriptor_t *device_descriptor_start;

	/* Points to the top of the device register area
	 * (next free byte) */
	uint32_t next_io_register;

	/* List of devices */
	device_t *devices;

	/* Sizes of the different segments.
	 * Sum up to size_total */
	uint32_t size_kseg0;
	uint32_t size_kseg1;
	uint32_t size_kseg2;
	uint32_t size_kuseg;
} mmu_t;

/* Reads from memory and writes to dst. Returns a relevant exception */
exception_t mmu_read(core_t *core, mmu_t *mem, int32_t vaddr, uint32_t *dst,
		     mem_op_size_t op_size);

/* Writes src to addr. Returns a relevant exception. */
exception_t mmu_write(core_t *core, mmu_t *mem, int32_t addr, uint32_t src,
		      mem_op_size_t op_size);

/* Translate virtual memory address to physical */
uint32_t translate_vaddr(uint32_t vaddr);

/* Translate physical memory address to actual */
uint8_t* translate_paddr(uint32_t paddr, mmu_t *mem);


/* Returns a new memory block */
mmu_t* mmu_init(size_t size);


/* Frees ressources used by mem */
void mmu_free(mmu_t *mem);


/* Adds a device descriptor to device descriptor memory area.
 * Will create a duplicate if device already exists! */
void device_descriptor_add(mmu_t *mmu, device_t *dev);

/* Adds a device to the MMU list and creates a descriptor for it */
void mmu_add_device(mmu_t *mmu, device_t *dev);

/* Find next free device descriptor */
device_descriptor_t* get_free_descriptor(mmu_t *mmu);


#endif /* _MEM_H */
