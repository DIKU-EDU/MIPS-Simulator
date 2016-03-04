#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "cpu.h"
#include "sim.h"

#include "tools.h"

int main(int argc, char **argv)
{
	/* Program to simulate */
	char *program = NULL;

	/* Number of cores. 1 by default*/
	int cores = 1;

	/* Parse command line arguments. */
	int c;
	while((c = getopt(argc, argv, "c:p:")) != -1) {
		switch(c) {
		case 'c':
			cores = atoi(optarg);
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
	return simulate(program);
}
