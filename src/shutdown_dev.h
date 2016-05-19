#ifndef _SHUTDOWN_DEV_H
#define _SHUTDOWN_DEV_H

#include <stdint.h>

#include "io.h"

#define SHUTDOWN_VENDOR_STRING	"SHUTDOWN"

/* Size of device registers */
#define IO_LENGTH_SHUTDOWN	0x04
#define POWEROFF_SHUTDOWN_MAGIC 0x0badf00d /* Defined in KUDOS */




device_t *shutdown_device_create();

int shutdown_device_write(device_t *dev, uint32_t addr, uint32_t data);

#endif /* _SHUTDOWN_DEV_H */


