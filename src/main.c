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

#define MEMORY_SIZE 0x20000000 /* 512 MiB */

static void
showUsage(const char *progname)
{
	printf("Usage: %s [options] <program>\n", progname);
}

static void
showHelp(const char *progname)
{
	showUsage(progname);
	printf("\n");
	printf("  -p, --program=PATH     "
		"path to the program to execute (MIPS32 ELF)\n");
	printf("  -c, --cores=NUMBER     "
		"the number of CPU cores to simulate\n");
	printf("  -m, --memory=NUMBER    "
		"the number of memory bytes to simulate\n");
	printf("  -d, --debug            "
		"print debug information during simulation\n");
}

static void
parse_size_t(size_t *target, const char *message)
{
	char *begin = optarg;
	*target = strtoul(begin, &optarg, 10);
	if (optarg == begin || (*target == ULONG_MAX && errno == ERANGE)) {
		fprintf(stderr, "%s\n", message);
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char **argv)
{
	/* Program to simulate */
	char *program = NULL;

	/* Debug flag */
	static int debug = 0;

	/* Number of cores. 1 by default*/
	size_t cores = 1;

	/* Memory size */
	size_t memsz = MEMORY_SIZE;

	/* Parse command line arguments. */

	static struct option
	long_options[] =
	{
		{"cores",    required_argument, 0, 'c'},
//		{"debug",    no_argument,       &debug, 1},
		{"debug",    no_argument,       0, 'c'},
		{"help",     no_argument,       0, 'h'},
		{"memory",   required_argument, 0, 'm'},
		{"program",  required_argument, 0, 'p'},
		{0, 0, 0, 0}
	};

	int c = 0;
	int option_index = 0;
	while((c = getopt_long(argc, argv, "+c:dhm:p:", long_options,
			       &option_index)
		) != -1) {

		switch(c) {

		case 0: // This is a flag, do nothing.
			break;

		case 'd':
			debug = 1;
			break;

		case 'c':
			parse_size_t(&cores, "Invalid number of cores.");
			break;

		case 'p':
			program = optarg;
			break;

		case 'm':
			parse_size_t(&memsz, "Invalid memory size.");
			break;

		case 'h':
			showHelp(argv[0]);
			exit(EXIT_SUCCESS);
			break;

		case '?':
		default:
			showUsage(argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	if (optind < argc) {
		program = argv[optind];
	}

	/* Exit if no program has been supplied*/
	if(program == NULL) {
		printf("No program supplied. Exitting.\n");
		return 0;
	}

	cores = cores; /* Ignore warning */
	return simulate(program, cores, memsz, debug);
}
