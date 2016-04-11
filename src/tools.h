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


#endif /* _TOOLS_H */
