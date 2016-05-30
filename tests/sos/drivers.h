#ifndef _DRIVERS_H
#define _DRIVERS_H

#include "types.h"

/* Device type codes */
#define TYPECODE_EMPTY		0x0000
#define TYPECODE_MEMINFO	0x0101
#define TYPECODE_RTC		0x0102
#define TYPECODE_SHUTDOWN	0x0103
#define TYPECODE_TTY		0x0201
#define TYPECODE_DISK		0x0301
#define TYPECODE_NIC		0x0401
#define TYPECODE_CPUINFO	0x0C00

/* Shutdown magic */
#define POWEROFF_SHUTDOWN_MAGIC 0x0badf00d

/* Device descriptor area start address */
#define IO_DESCRIPTOR_AREA		0xFFFE0000
#define IO_DESCRIPTOR_AREA_LENGTH	0xFFF10000

typedef struct {
	uint32_t typecode;
	uint32_t io_addr_base;
	uint32_t io_addr_len;
	uint32_t irq;
	char vendor_string[8];

	/* Reserved */
	uint32_t _reserved1;
	uint32_t _reserved2;
} __attribute__((packed)) device_descriptor_t;


void shutdown(uint32_t code);

#endif /* _DRIVERS_H */
