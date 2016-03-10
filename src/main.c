#include <errno.h>   // errno
#include <limits.h>  // ULONG_MAX
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "cpu.h"
#include "sim.h"

#include "tools.h"


/* Command-line options:
 *	-p <prog>	: Program to execute (MIPS32 ELF)
 *	-d		: debug
 *	-c <num>	: Number of cores
 */
#define OPTS "dc:p:"


int main(int argc, char **argv)
{
	/* Program to simulate */
	char *program = NULL;

	/* Debug flag */
	bool debug = false;

	/* Number of cores. 1 by default*/
	size_t cores = 1;

	/* Parse command line arguments. */
	int c;
	char *begin;
	while((c = getopt(argc, argv, "dc:p:")) != -1) {
		switch(c) {
		case 'd':
			debug = true;
			break;
		case 'c':
			begin = optarg;
			cores = strtoul(begin, &optarg, 10);
			if (begin == optarg || (cores == ULONG_MAX && errno == ERANGE)) {
				fprintf(stderr, "Invalid number of cores.\n");
				exit(EXIT_FAILURE);
			}
			break;
		case 'p':
			program = optarg;
			break;

		case '?':
			printf("USAGE: %s -p <program_name> [-c <cores>]\n",
			       argv[0]);
			return 0;
		}
	}

	/* Exit if no program has been supplied*/
	if(program == NULL) {
		printf("No program (-p <filename>) supplied. Exitting.\n");
		return 0;
	}

	cores = cores; /* Ignore warning */
	return simulate(program, debug);
}
