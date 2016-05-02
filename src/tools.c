#include <limits.h>
#include <stdint.h>
#include "tools.h"

uint32_t check_overflow(uint32_t a, uint32_t b)
{
	if (a > 0 && b > INT_MAX - a) {
		/* handle overflow */
		return 1;
	} else if (a < 0 && b < INT_MIN - a) {
		/* Underflow */
		return 0;
	}

	return 0;
}
