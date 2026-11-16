#include <stdio.h>

#define CTESTS_INTERNAL
#include "ct_internal.h"

// stdio buffers can hold output that has not reached the descriptor it is
// meant for yet. Flush every stream before reads and around descriptor swaps
// so that captured output and its ordering are what the caller expects.
void cti_fflush_step(t_capture *c)
{
	if (c)
		fflush(NULL);
}
