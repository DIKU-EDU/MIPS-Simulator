#include <stdio.h>

#include "debug.h"
#include "cpu.h"
#include "disasm.h"

void debug(uint32_t inst, core_t* core)
{
	print_instruction(inst, core);

	unsigned char c[3] = {0};
	bool stop = false;

	while(stop == false) {
		printf("> ");
		fgets((char*)c, 3, stdin);

		switch(c[0]) {
			/* Print all registers */
		case 'r':
			print_registers(core);
			break;

			/* Print instruction */
		case 'i':
			print_instruction(inst, core);
			break;

		case 'p':
			print_pipeline_registers(core);
			break;
			/* Print specified register only */
		case 'v':
		case 's':
		case 't':
			printf("%s = %u\n", c,
			       core->regs[register_to_number((char*)c)]);
			break;

			/* Continue */
		case 'c':
			stop = true;

		case '\n':
		default:
			break;
		}
	}
	printf("\n");
}


