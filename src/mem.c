#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "cpu.h"
#include "mem.h"
#include "exception.h"
#include "mips32.h"
#include "io.h"
#include "error.h"

mmu_t* mem_init(size_t size)
{
	mmu_t* mmu = (mmu_t*)calloc(1, sizeof(mmu_t));
	uint8_t* pmem = (uint8_t*)calloc(1, size);
	device_descriptor_t* dev_desc = (device_descriptor_t*)
					calloc(1, IO_DESCRIPTOR_AREA_LENGTH);


	if(mmu == NULL || pmem == NULL || dev_desc == NULL) {
		ERROR("Could not allocate memory.");
		exit(1);
	}

	mmu->device_descriptor_start = dev_desc;
	mmu->pmem = pmem;
	mmu->size_total = size;

	/* Point to start of IO REGISTER AREA */
	mmu->next_io_register = IO_REGISTER_AREA;

	mmu->size_kseg0 = mmu->size_kseg1 = size / 8;
	mmu->size_kseg2 = size / 4;
	mmu->size_kuseg = size / 2;


	DEBUG("KSEG0: 0x%08x\tKSEG1: 0x%08x\tKUSEG: 0x%08x",mmu->size_kseg0,
	      mmu->size_kseg1, mmu->size_kuseg);
	return mmu;
}


exception_t mem_read(core_t *core, mmu_t *mem, int32_t vaddr, uint32_t *dst,
		     mem_op_size_t op_size)
{
	/* Translated physical address */
	uint32_t paddr = translate_vaddr(vaddr);

	/* Translate to actual address*/
	uint8_t *aaddr = translate_paddr(paddr, mem);


	/* If I/O device */
	/* Reading any of the IO device registers in either usermode or
	 * supervisor mode when such accesses are not allowed, results in all
	 * zeros being returned */


	if(op_size == MEM_OP_BYTE) {
		*dst = GET_BIGBYTE(aaddr);
	} else if(op_size == MEM_OP_HALF) {
		*dst = GET_BIGHALF(aaddr);
	} else if(op_size == MEM_OP_WORD) {
		*dst = GET_BIGWORD(aaddr);
	}

	/*
	DEBUG("READ: 0x%08X FROM VADDR: 0x%08X, AADDR: %p", *((uint32_t*)dst), vaddr,
	      aaddr);
	*/

	return EXC_None;
}

exception_t mem_write(core_t *core, mmu_t *mem, int32_t vaddr, uint32_t src,
		      mem_op_size_t op_size)
{
	/* Translate to physical */
	uint32_t paddr = translate_vaddr(vaddr);

	/* Translate to actual address*/
	uint8_t *aaddr = translate_paddr(paddr, mem);


	/* write */
	if(op_size == MEM_OP_BYTE) {
		SET_BIGBYTE(aaddr, src);
	} else if(op_size == MEM_OP_HALF) {
		SET_BIGHALF(aaddr, src);
	} else if(op_size == MEM_OP_WORD) {
		SET_BIGWORD(aaddr, src);
	}

	/*
	DEBUG("WRITTEN: 0x%08X to VADDR: 0x%08X, AADDR: %p", *((uint32_t*)aaddr), vaddr,
	      aaddr);
	*/
	return EXC_None;
}

uint32_t translate_vaddr(uint32_t vaddr)
{
	uint32_t paddr = 0x00;

	/* See Mips Run p. 48 */
	/* I/O mapping */
	if(vaddr > IO_REGISTER_AREA) {
		return vaddr;

		/* io descriptor area */
	} else if(vaddr > IO_DESCRIPTOR_AREA) {
		/* TODO */

		/* kseg2 */
	} else if(vaddr > KSEG2_PSTART) {
		/* TODO: NOT MAPPED YET */
		return 0x00;
		/* kseg1 */
	} else if(vaddr > KSEG1_PSTART) {
		/* Strip off first 3 bits by ANDing:
		 *	1010  (0xA)
		 * AND  0001  (0x1)
		 * ----------------
		 *      0000  (0x0) */
		paddr = vaddr & 0x1FFFFFFF;
		/* kseg0 */
	} else if(vaddr > KSEG0_PSTART) {
		/* Strip off first bit by ANDing */
		paddr = vaddr & 0x7FFFFFFF;
		/* kuseg */
	}
	if(vaddr < KSEG0_PSTART) {
		paddr = vaddr + KSEG0_SIZE + KSEG1_SIZE;
	}

	return paddr;
}

uint8_t* translate_paddr(uint32_t paddr, mmu_t *mem)
{
	/* Actual address */
	uint8_t *aaddr = NULL;

	/* IO mapped address */
	if(paddr >= IO_REGISTER_AREA) {
		/* XXX: What to do here? */
		return NULL;

	} else if(paddr >= IO_DESCRIPTOR_AREA) {
		/* TODO */

	/* KSEG2 */
	} else if(paddr >= KSEG0_SIZE + KSEG1_SIZE + KUSEG_SIZE) {
		/* TODO */
		/* KUSEG */
	} else if(paddr >= KSEG1_SIZE + KSEG0_SIZE) {
		/* Check if out of bounds */
		if(paddr >= KUSEG_PSTART + mem->size_kseg0 + mem->size_kseg1 +
		   mem->size_kuseg) {
			/* TODO: Exception */
			ERROR("ADDRESS OVERFLOW, PADDR: 0x%08x. Out of bounds: 0x%08x",
			      paddr, KUSEG_PSTART + mem->size_kseg0 +
			      mem->size_kseg1 + mem->size_kuseg);
			return aaddr;
		}

		/* Calculate the actual address in the simulator */
		aaddr = mem->pmem + (paddr - KSEG1_PSTART - KSEG0_PSTART -
				     KUSEG_PSTART);

		/* KSEG0 */
	} else if(paddr >= KSEG1_SIZE) {
		/* Check if out of bounds */
		if(paddr >= KSEG0_PSTART + mem->size_kseg0 + mem->size_kseg1) {
			/* TODO: Exception */
			ERROR("ADDRESS OVERFLOW");
			return aaddr;
		}

		/* Calculate the actual address in the simulator */
		aaddr = mem->pmem + (paddr - KSEG1_PSTART - KSEG0_PSTART);

		/* KSEG1 */
	} else {
		/* Check if out of bounds */
		if(paddr >= KSEG1_PSTART + mem->size_kseg0) {
			/* TODO: Exception */
			ERROR("ADDRESS OVERFLOW");
			return aaddr;
		}

		/* Calculate the actual address in the simulator */
		aaddr = mem->pmem + (paddr - KSEG1_PSTART);
	}

	return aaddr;
}

void mem_free(mmu_t *mem)
{
	free(mem->pmem);
	free(mem);
}

void device_descriptor_add(mmu_t *mmu, device_t *dev)
{
	device_descriptor_t* dev_desc = get_free_descriptor(mmu);

	if(dev_desc == NULL) {
		ERROR("No free device descriptors found");
		return;
	}

	/* Set the fields */
	device_descriptor_set_fields(dev, dev_desc);

	/* Reverse the fields (MSB<->LSB) */
	device_descriptor_reverse(dev_desc);
}

void mmu_add_device(mmu_t *mmu, device_t *dev)
{
	/* Allocate io registers
	 * TODO: Check overflow */
	dev->io_addr_base = mmu->next_io_register;
	mmu->next_io_register += dev->io_addr_len;

	/* Point to the first device */
	dev->next = mmu->devices;

	/* Add device to the top of the list */
	mmu->devices = dev;

	/* Add device scriptor */
	device_descriptor_add(mmu, dev);
}

/* Find next free device descriptor */
device_descriptor_t *get_free_descriptor(mmu_t *mmu)
{
	size_t i = 0;
	for(i = 0; i < MAX_IO_DEVICES; i++) {
		device_descriptor_t dev = mmu->device_descriptor_start[i];
		device_descriptor_reverse(&dev);

		if(dev.typecode == TYPECODE_EMPTY) {
			return mmu->device_descriptor_start+i;
		}
	}

	/* None found */
	return NULL;
}
