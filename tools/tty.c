#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <curses.h>



/* Shared Memory Key */
#define SHM_KEY_TTY		0x4321		/* Just some magic number */

/* IO info */
#define IRQ_TTY			2
#define IO_LENGTH_TTY		12

/* IO device register offsets  */
#define TTY_REGISTER_STATUS	0x00000000
#define TTY_REGISTER_COMMAND	0x00000004
#define TTY_REGISTER_DATA	0x00000008



/* Switch between endianess */
#define REVERSE(x) \
	(((x) >> 24)&0x000000ff) | \
        (((x) <<  8)&0x00ff0000) | \
	(((x) >>  8)&0x0000ff00) | \
	(((x) << 24)&0xff000000)


int main(int argc, char **argv)
{
	/* Set SHM key */
	dev->shm_key = SHM_KEY_TTY;

	/* Create memory segment */
	if ((dev->shm_id = shmget(dev->shm_key, IO_LENGTH_TTY,
				  IPC_CREAT | 0666)) < 0) {
		printf("shmget");
		return;
	}

	/* Attach shared memory segment to our program */
	if ((dev->shm = shmat(dev->shm_id, NULL, 0)) == (char *) -1) {
		printf(shmat);
		exit(1);
	}


	WINDOW * mainwin;


	/*  Initialize ncurses  */

	if ( (mainwin = initscr()) == NULL ) {
		fprintf(stderr, "Error initialising ncurses.\n");
		exit(EXIT_FAILURE);
	}

	mvaddstr(13, 33, "Hello, world!");
	refresh();
	sleep(3);


	/*  Clean up after ourselves  */

	delwin(mainwin);
	endwin();
	refresh();

	return EXIT_SUCCESS;
}




}
