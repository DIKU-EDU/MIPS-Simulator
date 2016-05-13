#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

#include "exception.h"
#include "io.h"
#include "error.h"

io_device_t io_device_init(const char *fifo, uint32_t addr)
{
	io_device_t dev = {0};
	int fd;

	/* Unlink (delete) the existing file */
	if(unlink(fifo) == -1) {
		LOG("Could not unlink FIFO: %s", strerror(errno));
	}

	/* Create FIFO and check for error */
	if((mkfifo(fifo, 0666)) != 0) {
		ERROR("Could not create a new FIFO: %s", strerror(errno));
		return dev;
	}

	/* Open the FIFO */
	if((fd = open(fifo, O_RDWR)) == -1) {
		ERROR("Could not open a new FIFO: %s", strerror(errno));
		return dev;
	}

	dev.fd = fd;
	dev.addr = addr;

	DEBUG("IO DEVICES CREATED WITH FIFO: %s\tFD: %d\tAddr: 0x%08x", fifo,
	      fd, addr);

	return dev;
}

void io_device_free(io_device_t dev)
{
	if((close(dev.fd) == -1)) {
		ERROR("Could not close fd: %s", strerror(errno));
	}

	return;
}



exception_t io_read(io_device_t *dev)
{
	return EXC_None;
}

exception_t io_write(io_device_t *dev)
{
	return EXC_None;
}


