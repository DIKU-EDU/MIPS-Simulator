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


void device_descriptor_reverse(device_descriptor_t *dev)
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

void device_descriptor_set_fields(device_t *dev, device_descriptor_t *dev_desc)
{
	dev_desc->typecode		= dev->typecode;
	dev_desc->io_addr_base		= dev->io_addr_base;
	dev_desc->io_addr_len		= dev->io_addr_len;
	dev_desc->irq			= dev->irq;
	memcpy(dev_desc->vendor_string, dev->vendor_string, 8);
	dev_desc->_reserved1		= 0;
	dev_desc->_reserved2		= 0;
}
