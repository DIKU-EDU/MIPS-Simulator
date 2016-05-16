#ifndef _TTY_H
#define _TTY_H

#include "io.h"

/* TTY FIFO pipe */
#define IO_FIFO_TTY	".mips_sim_tty"


/* TTY device */
typedef struct tty_device {
	int fd;
	char *fifo;
} tty_device_t;


io_device_descriptor_t *tty_device_create();
int tty_device_read(io_device_descriptor_t *, uint32_t, uint32_t*);
int tty_device_write(io_device_descriptor_t *, uint32_t, uint32_t);


#endif /* _TTY_H */
