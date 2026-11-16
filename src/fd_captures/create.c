#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define CTESTS_INTERNAL
#include "ct_internal.h"

// Make reads on the capture pipe non blocking: get/compare then only consume
// what is already there instead of waiting for output that may never come.
static bool cti_capture_nonblock(t_capture *capture)
{
	int flags;

	flags = fcntl(capture->pipe_read, F_GETFL, 0);
	if (flags == -1)
		return (false);
	return (fcntl(capture->pipe_read, F_SETFL, flags | O_NONBLOCK) != -1);
}

// Initialise capture on source_fd. On failure the capture is left released
// (all descriptors set to -1) so that ct_capture_release() can be called again
// safely.
bool ct_capture_init(int source_fd, t_capture *capture)
{
	int pipe_fds[2];

	if (!capture)
		return (false);
	memset(capture, 0, sizeof(*capture));
	capture->original_fd = -1;
	capture->source_fd = source_fd;
	capture->pipe_read = -1;
	capture->pipe_write = -1;
	if (source_fd < 0)
		return (false);
	capture->original_fd = dup(source_fd);
	if (capture->original_fd == -1)
		return (false);
	// Flush stdio buffers before they are redirected: pending output belongs
	// to the descriptor the caller had before the capture started.
	cti_fflush_step(capture);
	if (pipe(pipe_fds) == -1)
		return (ct_capture_release(capture), false);
	capture->pipe_read = pipe_fds[0];
	capture->pipe_write = pipe_fds[1];
	if (dup2(capture->pipe_write, source_fd) == -1)
		return (ct_capture_release(capture), false);
	close(capture->pipe_write);
	capture->pipe_write = -1;
	if (!cti_capture_nonblock(capture))
		return (ct_capture_release(capture), false);
	return (true);
}

/// @brief Start capturing everything written to @p source_fd.
/// @details The descriptor is duplicated and redirected to an internal pipe.
/// The original descriptor is restored by ct_capture_destroy().
/// @param source_fd Descriptor to capture, usually STDOUT_FILENO or
/// STDERR_FILENO.
/// @return The capture handle, or NULL when the descriptor can not be
/// redirected or the allocation fails.
t_capture *ct_capture_create(int source_fd)
{
	t_capture *tmp;

	tmp = safealloc(sizeof(*tmp));
	if (!tmp)
		return (NULL);
	if (!ct_capture_init(source_fd, tmp))
		return (safefree(tmp), NULL);
	return (tmp);
}
