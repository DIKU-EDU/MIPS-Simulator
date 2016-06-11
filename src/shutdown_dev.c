#include <string.h>

#include "sim.h"
#include "error.h"
#include "io.h"
#include "shutdown_dev.h"

device_t* shutdown_device_create()
{
	device_t *dev = calloc(1, sizeof(struct _device_t));
	shutdown_device_t *shutdown_dev = calloc(1, sizeof(struct
							   shutdown_device));

	if(dev == NULL || shutdown_dev == NULL) {
		ERROR("Could not alloc shutdown device");
		return NULL;
	}

	dev->typecode = TYPECODE_SHUTDOWN;

	memcpy(dev->vendor_string, SHUTDOWN_VENDOR_STRING, 8);

	dev->irq = IRQ_INVALID;
	dev->io_write = &shutdown_device_write;

	dev->io_addr_len = IO_LENGTH_SHUTDOWN;

	dev->realdevice = (void *)shutdown_dev;

	return dev;
}

int shutdown_device_write(device_t *dev, uint32_t addr, uint32_t data)
{
	DEBUG("SHUTDOWN WRITTEN");

	if(data == POWEROFF_SHUTDOWN_MAGIC) {
		((shutdown_device_t*)dev->realdevice)->shutdown_triggered = true;

		/* XXX: How to turn off? */
		LOG("SHUTDOWN SIGNAL RECEIVED");
		exit(0);
	}
	return 0;
}

