#include "drivers.h"

void init(void)
{
	shutdown(POWEROFF_SHUTDOWN_MAGIC);

	while(1)
		;
}
