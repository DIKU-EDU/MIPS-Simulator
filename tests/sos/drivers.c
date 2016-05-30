#include "types.h"
#include "drivers.h"

void shutdown(uint32_t code)
{
	/* Find the device */
	device_descriptor_t *dev = (device_descriptor_t *)IO_DESCRIPTOR_AREA;

	while((uint32_t)dev < IO_DESCRIPTOR_AREA + IO_DESCRIPTOR_AREA_LENGTH) {
		if(dev->typecode == TYPECODE_SHUTDOWN) {
			break;
		}

		dev++;
	}

	*(uint32_t *)(dev->io_addr_base) = code;
}
