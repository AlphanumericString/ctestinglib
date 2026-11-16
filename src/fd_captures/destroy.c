#define CTESTS_INTERNAL
#include "ct_internal.h"

void ct_capture_destroy(t_capture *c)
{
	if (!c)
		return;
	ct_capture_release(c);
	safefree(c);
}
