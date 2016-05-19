#include "io.h"
#include "shutdown_dev.h"

int shutdown_device_init(device_descriptor_t *dev)
{
	dev->device_type = TYPECODE_SHUTDOWN;
	memcpy(dev->vendor_string, "SHUTDOWN", 8);
	dev->irq = IRQ_INVALID;
	dev->io_write = &shutdown_device_write;

	dev->io_addr_len = IO_LENGTH_SHUTDOWN;

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



