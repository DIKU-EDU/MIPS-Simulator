#include <stdint.h>

#include "tty.h"
#include "io.h"


io_device_descriptor_t *tty_device_create()
{
	io_device_descriptor_t *dev = malloc(sizeof(io_device_descriptor_t));
	if(dev == NULL) {
		ERROR("Could not allocate tty device IO.");
		return NULL;
	}

	dev->device_type = TYPECODE_TTY;
	memcpy(dev->vendor_string, "TTY     ", 8);
	dev->irq = IRQ_TTY;


	tty_device_t *tty_dev = malloc(sizeof(tty_device_t));
	if(dev->device == NULL) {
		ERROR("Could not allocate tty_device_t");
		return NULL;
	}

	dev->io_write = &tty_device_write;
	dev->io_read = &tty_device_read;

	dev->addr_base = 0; /* XXX? */
	dev->io_length = 0; /* XXX? */

	/* Unlink (delete) the existing file */
	if(unlink(IO_FIFO_TTY) == -1) {
		LOG("Could not unlink FIFO: %s", strerror(errno));
	}

	/* Create FIFO and check for error */
	if((mkfifo(IO_FIFO_TTY, 0666)) != 0) {
		ERROR("Could not create a new FIFO: %s", strerror(errno));
		return dev;
	}

	/* Open the FIFO */
	if((tty_dev->fd = open(IO_FIFO_TTY, O_RDWR)) == -1) {
		ERROR("Could not open a new FIFO: %s", strerror(errno));
		return	dev;
	}

	tty_dev->fifo = IO_FIFO_TTY;

	dev->device = (void*)tty_dev;

	return dev;
}

int tty_device_read(io_device_descriptor_t *, uint32_t, uint32_t*)
{

}

int tty_device_write(io_device_descriptor_t *, uint32_t, uint32_t)
{


}
