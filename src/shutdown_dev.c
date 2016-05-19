#include <string.h>

#include "io.h"
#include "shutdown_dev.h"


/* extern'd in sim.c */
extern bool g_finished;

device_t* shutdown_device_init(device_t *dev)
{
	dev->typecode = TYPECODE_SHUTDOWN;
	memcpy(dev->vendor_string, "SHUTDOWN", 8);
	dev->irq = IRQ_INVALID;
	dev->io_write = &shutdown_device_write;

	dev->io_addr_len = IO_LENGTH_SHUTDOWN;

	return dev;
}

int shutdown_device_write(device_t *dev, uint32_t addr, uint32_t data)
{
	if(data == POWEROFF_SHUTDOWN_MAGIC) {
		g_finished = true;
	}
	return 0;
}



