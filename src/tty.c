#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "error.h"
#include "tty.h"
#include "io.h"

/* Sets-up shared memory for the tty_device */
static void tty_device_init(tty_device_t *dev)
{
	/* Set SHM key */
	dev->shm_key = SHM_KEY_TTY;

	/* Create memory segment */
	if ((dev->shm_id = shmget(dev->shm_key, IO_LENGTH_TTY,
						IPC_CREAT | 0666)) < 0) {
		ERROR("shmget failed: %s", strerror(errno));
		return;
	}

	/* Attach shared memory segment to our program */
	if ((dev->shm = shmat(dev->shm_id, NULL, 0)) == (char *) -1) {
		ERROR("shmat failed: %s", strerror(errno));
		exit(1);
	}

	/* device_t should now be fully set up */
}


device_t *tty_device_create()
{
	device_t *dev = calloc(1, sizeof(device_t));

	if(dev == NULL) {
		ERROR("Could not allocate tty device IO.");
		return NULL;
	}

	dev->typecode = TYPECODE_TTY;
	memcpy(dev->vendor_string, TTY_VENDOR_STRING, 8);
	dev->irq = IRQ_TTY;
	dev->io_addr_len = IO_LENGTH_TTY;
	dev->io_write = &tty_device_write;
	dev->io_read = &tty_device_read;
	dev->io_addr_base = 0; /* NOTE: Set in the simulator */


	tty_device_t *tty_dev = malloc(sizeof(tty_device_t));
	if(tty_dev == NULL) {
		ERROR("Could not allocate tty_device_t");
		return NULL;
	}


	/* Init the tty_device */
	tty_device_init(tty_dev);
	dev->realdevice = (void*)tty_dev;


	return dev;
}

int tty_device_read(device_t *dev, uint32_t addr, uint32_t* data)
{
	return -1;

}

int tty_device_write(device_t *dev, uint32_t addr, uint32_t data)
{


	return -1;

}
