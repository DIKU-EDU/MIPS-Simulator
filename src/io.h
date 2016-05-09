#ifndef _IO_H
#define _IO_H

#include <stdint.h>
#include <stdbool.h>

/* Receiver Control */
#define RCTRL_READY		(uint32_t)(1 << 0)
#define RCTRL_INTERRUPT_ENABLE	(uint32_t)(1 << 1)

/* Receiver Data */
#define RDATA_BYTE		(uint32_t)(0xFF)

/* Transmitter Control */
#define TCTRL_READY		(uint32_t)(1 << 0)
#define TCTRL_INTERRYPT_ENABLE	(uint32_t)(1 << 1)

/* Transmitter Data */
#define TDATA_BYTE		(uint32_t)(0xFF)

/* IO device structure */
typedef struct io_device {
	char *fifo;
} io_device_t;

/* Named Pipes names */
#define IO_FIFO_KEYBOARD	"/tmp/.mips_sim_keyboard"
#define IO_FIFO_DISPLAY		"/tmp/.mips_sim_display"

/* Simulator pipe structure */
typedef struct pipe_io {
	bool ready;
	bool interrupt_enable;
	uint8_t byte;
} pipe_io_t;


/* Reads from an IO device and writes to the appropriate IO mapped memory */
void io_read(io_device_t *dev);


/* Writes to an IO device */
void io_write(io_device_t *dev);

#endif /* _IO_H */
