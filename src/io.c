#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

#include "sim.h"
#include "exception.h"
#include "io.h"
#include "error.h"

/* extern'd in sim.c */
extern bool g_finished;

io_device_descriptor_t *shutdown_device_init()
{
	io_device_descriptor_t *dev = malloc(sizeof(struct io_device_descriptor));
	if(dev == NULL) {
		ERROR("Could not allocate shutdown io device.");
		return dev;
	}

	dev->device_type = TYPECODE_SHUTDOWN;
	memcpy(dev->vendor_string, "SHUTDOWN", 8);
	dev->irq = IRQ_INVALID;
	dev->io_write = &shutdown_device_write;


	return dev;
}

int shutdown_device_write(io_device_descriptor_t *dev, uint32_t addr,
			  uint32_t data)
{
	if(data == POWEROFF_SHUTDOWN_MAGIC) {
		g_finished = true;
	}
	return 0;
}


void reverse_device_descriptor(device_descriptor_t *dev)
{
	/* Reverse the fields */
	dev->typecode		= REVERSE(dev->typecode);
	dev->io_addr_base	= REVERSE(dev->io_addr_base);
	dev->io_addr_len	= REVERSE(dev->io_addr_len);
	dev->irq		= REVERSE(dev->irq);

	/* Reverse the reserved fields as well, just to be sure */
	dev->_reserved1		= REVERSE(dev->_reserved1);
	dev->_reserved2		= REVERSE(dev->_reserved2);
}


