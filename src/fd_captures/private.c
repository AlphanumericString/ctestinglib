#include <unistd.h>
#include <stdio.h>

#define CTESTS_INTERNAL
#include "ct_internal.h"

void cti_fflush_step(t_capture *c)
{
	if (c->source_fd == STDOUT_FILENO)
		fflush(stdout);
	else if (c->source_fd == STDERR_FILENO)
		fflush(stderr);
}
