#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "io.h"
#include "error.h"

io_device_t io_device_init(const char *fifo, uint32_t addr)
{
	io_device_t dev = {0};

	int fd;

	/* Create FIFO and check for error */
	if((fd = mkfifo(fifo, O_RDWR)) != 0) {
		ERROR("%s", strerror(errno));
		return dev;
	}

	dev.fd = fd;
	dev.addr = addr;

	return dev;
}


void io_read(io_device_t *dev)
{
}

void io_write(io_device_t *dev)
{
}


