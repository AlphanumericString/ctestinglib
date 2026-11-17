#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CTESTS_INTERNAL
#include "ct_internal.h"

#define CT_CAPTURE_TMPDIR_DEFAULT "/tmp"
#define CT_CAPTURE_TMPNAME "/ct_capture_XXXXXX"
#define CT_CAPTURE_PATH_MAX 256

// Create an already unlinked temporary file: it only lives as long as one of
// its descriptors is open and never shows up in the filesystem.
static int cti_capture_tmpfd(void)
{
	char		path[CT_CAPTURE_PATH_MAX];
	const char *dir;
	size_t		dir_len;
	size_t		name_len;
	int			fd;

	dir = getenv("TMPDIR");
	if (!dir || dir[0] == '\0')
		dir = CT_CAPTURE_TMPDIR_DEFAULT;
	dir_len = strlen(dir);
	name_len = strlen(CT_CAPTURE_TMPNAME);
	if (dir_len + name_len + 1 > sizeof(path))
		return (-1);
	memcpy(path, dir, dir_len);
	memcpy(path + dir_len, CT_CAPTURE_TMPNAME, name_len + 1);
	fd = mkstemp(path);
	if (fd == -1)
		return (-1);
	if (unlink(path) == -1)
		return (close(fd), -1);
	return (fd);
}

static bool cti_capture_write_input(int fd, const char *input)
{
	size_t	len;
	size_t	written;
	ssize_t n;

	len = strlen(input);
	written = 0;
	while (written < len)
	{
		n = write(fd, input + written, len - written);
		if (n < 0)
		{
			if (errno == EINTR)
				continue;
			return (false);
		}
		if (n == 0)
			return (false);
		written += (size_t)n;
	}
	return (lseek(fd, 0, SEEK_SET) != -1);
}

/// @brief Feed the stdin of the current test.
/// @details The captured stdin is replaced by a temporary file containing
/// @p input. Reading stdin then returns exactly @p input and reaches
/// end-of-file afterwards, without waiting for more data. The function can be
/// called several times on the same capture: each call replaces the
/// descriptor, so descriptor-level reads never see input from a previous call.
/// A FILE* stream such as stdin may still hold data it buffered before the
/// replacement.
/// @param capture Capture created on STDIN_FILENO, NULL is accepted.
/// @param input NUL terminated input to make available on stdin, NULL is
/// accepted.
/// @return 0 on success, -1 when an argument is NULL, when @p capture does not
/// capture STDIN_FILENO, or when the input can not be written.
int ct_capture_feed(t_capture *capture, const char *input)
{
	int fd;

	if (!capture || !input || capture->original_fd == -1 ||
		capture->source_fd != STDIN_FILENO)
		return (-1);
	fd = cti_capture_tmpfd();
	if (fd == -1)
		return (-1);
	if (!cti_capture_write_input(fd, input))
		return (close(fd), -1);
	if (dup2(fd, capture->source_fd) == -1)
		return (close(fd), -1);
	close(fd);
	// The pipe created by ct_capture_create(STDIN_FILENO) is no longer used
	// once a file feeds stdin: release it so that stdin reads the input file.
	if (capture->pipe_read != -1)
	{
		close(capture->pipe_read);
		capture->pipe_read = -1;
	}
	if (capture->pipe_write != -1)
	{
		close(capture->pipe_write);
		capture->pipe_write = -1;
	}
	// A previous case may have reached EOF on stdin: reset the stream state so
	// that the new input can be read by scanf()/fgets() callers.
	clearerr(stdin);
	return (0);
}

/// @brief Capture STDIN_FILENO and feed it @p input.
/// @details Equivalent to ct_capture_create(STDIN_FILENO) followed by
/// ct_capture_feed(). The original stdin is restored by ct_capture_destroy().
/// @param input NUL terminated input the code under test should read, NULL is
/// accepted.
/// @return The capture handle, or NULL when @p input is NULL, when stdin can
/// not be redirected or when the allocation fails.
t_capture *ct_capture_stdin(const char *input)
{
	t_capture *capture;

	if (!input)
		return (NULL);
	capture = ct_capture_create(STDIN_FILENO);
	if (!capture)
		return (NULL);
	if (ct_capture_feed(capture, input) != 0)
		return (ct_capture_destroy(capture), NULL);
	return (capture);
}
