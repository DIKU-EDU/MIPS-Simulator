#include <errno.h>   // errno
#include <limits.h>  // ULONG_MAX
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <getopt.h>

#include "cpu.h"
#include "sim.h"
#include "mem.h"
#include "tools.h"
#include "error.h"

/* Command-line options:
 *	-p <prog>	: Program to execute (MIPS32 ELF)
 *	-d		: debug
 *	-c <num>	: Number of cores
 *      -m <bytes>        : Size of memory in bytes
 */
#define OPTS "dc:p:m:"
#define MEMORY_SIZE 0x20000000 /* 512 MiB */

int main(int argc, char **argv)
{
	/* Program to simulate */
	char *program = NULL;

	/* Debug flag */
	static int debug = 0;

	/* Number of cores. 1 by default*/
	size_t cores = 1;

	/* Memory size */
	size_t mem = MEMORY_SIZE;

	/* Parse command line arguments. */

	static struct option
	long_options[] =
	{
		{"cores",    required_argument, 0, 'c'},
		{"debug",    no_argument,       &debug, 1},
		{"help",     no_argument,       0, 'h'},
		{"memory",   required_argument, 0, 'm'},
		{"prorgram", required_argument, 0, 'p'},
		{0, 0, 0, 0}
	};

	int c = 0;
	int option_index = 0;
	char *begin = 0;
	while((c = getopt_long(argc, argv, "+c:dhm:p:", long_options, &option_index)
		) != -1) {

		switch(c) {

		case 0: // This is a flag, do nothing.
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

		case 'm':
			begin = optarg;
			mem = strtoul(begin, &optarg, 10);
			if (begin == optarg || (cores == ULONG_MAX && errno == ERANGE)) {
				fprintf(stderr, "Invalid memory size.\n");
				exit(EXIT_FAILURE);
			}
			break;

		case 'h':
		case '?':
		default:
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
	return simulate(program, cores, mem, debug);
}
