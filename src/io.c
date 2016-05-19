#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

#include "sim.h"
#include "exception.h"
#include "mem.h"
#include "io.h"
#include "error.h"

/* extern'd in sim.c */
extern bool g_finished;

/* Find next free device descriptor */
device_descriptor_t *get_free_descriptor(mmu_t *mmu)
{
	size_t i = 0;
	for(i = 0; i < NUM_IO_DEVICES; i++) {
		device_descriptor_t dev = REVERSE(
					mmu->device_descriptor_start[i]);

		if(dev.typecode == TYPECODE_EMPTY) {
			return mmu->device_descriptor_start+i;
		}
	}

	/* None found */
	return NULL;
}

void reverse_device_descriptor(device_descriptor_t *dev)
{
	/* Reverse the fields */
	dev->typecode		= REVERSE(dev->typecode);
	dev->io_addr_base	= REVERSE(dev->io_addr_base);
	dev->io_addr_len	= REVERSE(dev->io_addr_len);
	dev->irq		= REVERSE(dev->irq);

	/* vendor_string need not be reversed */

	/* Reverse the reserved fields as well, just to be sure */
	dev->_reserved1		= REVERSE(dev->_reserved1);
	dev->_reserved2		= REVERSE(dev->_reserved2);
}




