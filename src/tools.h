#ifndef _TOOLS_H
#define _TOOLS_H

#include <stdint.h>


/* Switch between endianess */
#define REVERSE(x) \
	(((x) >> 24)&0x000000ff) | \
        (((x) <<  8)&0x00ff0000) | \
	(((x) >>  8)&0x0000ff00) | \
	(((x) << 24)&0xff000000)

/* Bit flipper for easier bitlevel masking  */
#define BIT(x) (uint32_t)(1 << (x))


/* Function that will detect overflow on unsigned integers
 * Returns 1 if overflow occurs, 0 otherwise */
uint32_t check_uoverflow(uint32_t a, uint32_t b);


/* Function to detect overflow and underflow on signed integers */
/* Returns 1 if under- or overflow will occur, 0 otherwise. */
uint32_t check_soverflow(int32_t a, int32_t b);


#endif /* _TOOLS_H */
