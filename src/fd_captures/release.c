#include <stdio.h>
#include <unistd.h>

#define CTESTS_INTERNAL
#include "ct_internal.h"

// Flush stdio, restore the captured descriptor and close every resource held
// by the capture. The structure is left empty (all descriptors set to -1 and
// the pending buffer freed), so calling release twice is harmless and returns
// true.
bool ct_capture_release(t_capture *capture)
{
	bool restored;
	bool was_stdin;

	if (!capture)
		return (false);
	restored = true;
	was_stdin = (capture->source_fd == STDIN_FILENO);
	// Flush stdio before restoring the descriptor: output still sitting in
	// stdio buffers belongs to the capture and must not leak to the real
	// descriptor once it is restored. It is discarded with the capture pipe.
	cti_fflush_step(capture);
	if (capture->original_fd != -1)
	{
		if (capture->source_fd == -1 ||
			dup2(capture->original_fd, capture->source_fd) == -1)
			restored = false;
		close(capture->original_fd);
	}
	if (capture->pipe_read != -1)
		close(capture->pipe_read);
	if (capture->pipe_write != -1)
		close(capture->pipe_write);
	safefree(capture->pending);
	capture->pending = NULL;
	capture->pending_len = 0;
	capture->pending_cap = 0;
	capture->original_fd = -1;
	capture->source_fd = -1;
	capture->pipe_read = -1;
	capture->pipe_write = -1;
	// The previous case may have reached EOF or another error on stdin: the
	// restored stream must start from a clean state.
	if (was_stdin)
		clearerr(stdin);
	return (restored);
}
