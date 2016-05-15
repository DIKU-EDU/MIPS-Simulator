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

/* IO device descriptor structure
 * YAMS docs, section 6.4.1 Device descriptors */
typedef struct io_device_descriptor {
	uint32_t device_type;
	uint32_t addr_base;
	uint32_t io_length;
	uint32_t irq;
	char vendor_string[8];

	uint64_t reserved;

	int (*io_read)(struct io_device_descriptor, uint32_t addr, uint32_t *data);
	int (*io_write)(struct io_device_descriptor, uint32_t addr, uint32_t data);
	int (*tick)();
}__attribute__((packed)) io_device_descriptor_t;


/* Named Pipes names */
#define IO_FIFO_KEYBOARD	".mips_sim_keyboard"
#define IO_FIFO_DISPLAY		".mips_sim_display"

/* Definitions for IO devices. This should probably be dynamic */
#define IO_DEVICE_KEYBOARD	0x00
#define IO_DEVICE_DISPLAY	0x01


/* Simulator pipe structure */
typedef struct pipe_io {
	bool ready;
	bool interrupt_enable;
	uint8_t byte;
} pipe_io_t;

#endif /* _IO_H */
