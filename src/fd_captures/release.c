#include <unistd.h>

#define CTESTS_INTERNAL
#include "ct_internal.h"

// Restore the captured descriptor and close every resource held by the
// capture. The structure is left empty (all descriptors set to -1), so calling
// release twice is harmless and returns true.
bool ct_capture_release(t_capture *capture)
{
	bool restored;

	if (!capture)
		return (false);
	restored = true;
	if (capture->original_fd != -1)
	{
		if (capture->source_fd == -1 ||
			dup2(capture->original_fd, capture->source_fd) == -1)
			restored = false;
		close(capture->original_fd);
	}
	// Restore the descriptor first so that output still sitting in stdio
	// buffers goes to the real destination instead of the closing pipe.
	cti_fflush_step(capture);
	if (capture->pipe_read != -1)
		close(capture->pipe_read);
	if (capture->pipe_write != -1)
		close(capture->pipe_write);
	capture->original_fd = -1;
	capture->source_fd = -1;
	capture->pipe_read = -1;
	capture->pipe_write = -1;
	return (restored);
}
