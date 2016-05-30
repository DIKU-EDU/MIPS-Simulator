#include <string.h>

#include "error.h"
#include "io.h"
#include "shutdown_dev.h"


/* extern'd in sim.c */
extern bool g_finished;

device_t* shutdown_device_create()
{
	device_t *dev = calloc(1, sizeof(struct _device_t));

	if(dev == NULL) {
		ERROR("Could not alloc shutdown device");
		return NULL;
	}

	dev->typecode = TYPECODE_SHUTDOWN;

	memcpy(dev->vendor_string, SHUTDOWN_VENDOR_STRING, 8);

	dev->irq = IRQ_INVALID;
	dev->io_write = &shutdown_device_write;

	dev->io_addr_len = IO_LENGTH_SHUTDOWN;

	return dev;
}

int shutdown_device_write(device_t *dev, uint32_t addr, uint32_t data)
{
	DEBUG("SHUTDOWN WRITTEN");

	if(data == POWEROFF_SHUTDOWN_MAGIC) {
		g_finished = true;
	}
	return 0;
}

