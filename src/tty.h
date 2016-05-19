#ifndef _TTY_H
#define _TTY_H
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#include "io.h"

/* Shared Memory Key */
#define SHM_KEY_TTY		0x4321		/* Just some magic number */

/* IO info */
#define IRQ_TTY			2
#define IO_LENGTH_TTY		12

/* TTY vendor string */
#define TTY_VENDOR_STRING	"TTY     "


/* TTY device */
typedef struct tty_device {
	key_t shm_key;
	int shm_id;
	char *shm;	/* Pointer to the shared memory */
} tty_device_t;


device_t *tty_device_create();
int tty_device_read(device_t *, uint32_t, uint32_t*);
int tty_device_write(device_t *, uint32_t, uint32_t);


#endif /* _TTY_H */
