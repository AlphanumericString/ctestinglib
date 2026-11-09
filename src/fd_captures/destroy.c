#include "ct_capture.h"
#define CTESTS_INTERNAL
#include "ct_memory.h"

void	ct_capture_destroy(t_capture *c)
{
	if (!c)
		return ;
	ct_capture_release(c);
	safefree(c);
}
