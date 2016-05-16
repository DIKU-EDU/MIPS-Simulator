#ifndef _IO_H
#define _IO_H

#include <stdint.h>
#include <stdbool.h>

#include "exception.h"

/* Receiver Control */
#define RCTRL_OFFSET		(0x00000000)
#define RCTRL_READY		(uint32_t)(1 << 0)
#define RCTRL_INTERRUPT_ENABLE	(uint32_t)(1 << 1)

/* Receiver Data */
#define RDATA_OFFSET		(0x00000004)
#define RDATA_BYTE		(uint32_t)(0xFF)

/* Transmitter Control */
#define TCTRL_OFFSET		(0x00000008)
#define TCTRL_READY		(uint32_t)(1 << 0)
#define TCTRL_INTERRYPT_ENABLE	(uint32_t)(1 << 1)

/* Transmitter Data */
#define TDATA_OFFSET		(0x0000000C)
#define TDATA_BYTE		(uint32_t)(0xFF)

/* Device type codes */
#define TYPECODE_MEMINFO	0x101
#define TYPECODE_RTC		0x102
#define TYPECODE_SHUTDOWN	0x103
#define TYPECODE_TTY		0x201
#define TYPECODE_DISK		0x301
#define TYPECODE_NIC		0x401
#define TYPECODE_CPUINFO	0xC00

/* IRQs */
#define IRQ_INVALID		(-1)


/* descriptor structure
 * YAMS docs, section 6.4.1 Device descriptors */
typedef struct io_device_descriptor {
	uint32_t device_type;
	uint32_t addr_base;
	uint32_t io_length;
	uint32_t irq;
	char vendor_string[8];

	uint64_t reserved;

	/* Pointer to a structure specific for the actual device */
	void *device;

	int (*io_read)(struct io_device_descriptor*, uint32_t, uint32_t *);
	int (*io_write)(struct io_device_descriptor*, uint32_t, uint32_t);
	int (*tick)();
}__attribute__((packed)) io_device_descriptor_t;


/* Simulator pipe structure */
typedef struct pipe_io {
	bool ready;
	bool interrupt_enable;
	uint8_t byte;
} pipe_io_t;


/* Shutdown device */
#define POWEROFF_SHUTDOWN_MAGIC 0x0badf00d /* Defined in KUDOS */

io_device_descriptor_t *shutdown_device_init();

int shutdown_device_write(io_device_descriptor_t *dev, uint32_t addr,
			  uint32_t data);

#endif /* _IO_H */
