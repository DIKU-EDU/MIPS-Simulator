#include <limits.h>
#include <stdint.h>
#include "tools.h"

uint32_t check_uoverflow(uint32_t a, uint32_t b)
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


uint32_t check_soverflow(int32_t a, int32_t b)
{
	if ((b > 0) && (a > INT_MAX - b)
	    || (b < 0) && (a < INT_MIN - b)) {
		return 1;
	}

	return 0;

}


