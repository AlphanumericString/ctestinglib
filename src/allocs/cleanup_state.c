#define CTESTS_INTERNAL
#include "ct_memory.h"

static bool *cti_memclnup_state(void)
{
	static bool state = false;

	return (&state);
}

void cti_set_memory_cleanup(bool val)
{
	*cti_memclnup_state() = val;
}

bool cti_get_memory_cleanup(void)
{
	return (*cti_memclnup_state());
}
