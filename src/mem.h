#ifndef _MEM_H
#define _MEM_H

typedef struct memory {
	uint8_t *raw;	/* Raw memory */
	uint32_t size;  /* Allocated memory */
} memory_t;


/* Returns a new memory block */
memory_t* mem_init(size_t size);

/* Frees ressources used by mem */
void mem_free(memory_t *mem);


#endif /* _MEM_H */
