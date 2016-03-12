#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#include "mem.h"

memory_t* mem_init(size_t size)
{
	memory_t* memory = (memory_t*)calloc(1, sizeof(memory_t));
	uint8_t* raw = (uint8_t*)calloc(1, size);

	if(memory == NULL || raw == NULL) {
		printf("Could not allocate memory.\n");
		exit(1);
	}

	memory->raw = raw;
	memory->size = size;
	return memory;
}


void mem_free(memory_t *mem)
{
	free(mem->raw);
	free(mem);
}


