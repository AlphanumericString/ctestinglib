#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define CTESTS_INTERNAL
#include "ct_internal.h"

// Make room for at least extra more bytes in the pending buffer. One extra
// byte is always reserved so that get() can append a NUL terminator.
static bool cti_capture_reserve(t_capture *capture, size_t extra)
{
	size_t needed;
	size_t new_capacity;
	char  *grown;

	if (capture->pending_len >= SIZE_MAX - extra)
		return (false);
	needed = capture->pending_len + extra + 1;
	if (needed <= capture->pending_cap)
		return (true);
	new_capacity =
		capture->pending_cap ? capture->pending_cap : CT_RBUFFSIZE + 1;
	while (new_capacity < needed)
	{
		if (new_capacity > SIZE_MAX / 2)
			return (false);
		new_capacity *= 2;
	}
	grown = safealloc(new_capacity);
	if (!grown)
		return (false);
	if (capture->pending)
	{
		memcpy(grown, capture->pending, capture->pending_len);
		safefree(capture->pending);
	}
	capture->pending = grown;
	capture->pending_cap = new_capacity;
	return (true);
}

// Move everything currently available on the capture pipe into the pending
// buffer. The read end is non blocking, so a short read means "nothing else
// was written yet", not an error. The data stays pending until a consuming
// function (get/compare) is called.
static bool cti_capture_fill(t_capture *capture)
{
	char	chunk[CT_RBUFFSIZE];
	ssize_t n;

	for (;;)
	{
		n = read(capture->pipe_read, chunk, sizeof(chunk));
		if (n < 0)
		{
			if (errno == EINTR)
				continue;
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				return (true);
			return (false);
		}
		if (n == 0)
			return (true);
		if (!cti_capture_reserve(capture, (size_t)n))
			return (false);
		memcpy(capture->pending + capture->pending_len, chunk, (size_t)n);
		capture->pending_len += (size_t)n;
	}
}

// Drop the pending data once it has been consumed.
static void cti_capture_consume(t_capture *capture)
{
	capture->pending_len = 0;
}

/// @brief Consume the output written since the last call.
/// @details Reads everything currently available on the capture pipe without
/// blocking. The returned string is NUL terminated and is truncated at the
/// first NUL byte.
/// @param capture Capture handle, NULL is accepted.
/// @return A malloc'ed string to free with free() or safefree(), an empty
/// string when nothing was written, or NULL on error or when @p capture is
/// NULL.
char *ct_capture_get(t_capture *capture)
{
	char *data;

	if (!capture || capture->pipe_read == -1)
		return (NULL);
	cti_fflush_step(capture);
	if (!cti_capture_fill(capture))
		return (NULL);
	data = safealloc(capture->pending_len + 1);
	if (!data)
		return (NULL);
	if (capture->pending_len)
		memcpy(data, capture->pending, capture->pending_len);
	data[capture->pending_len] = '\0';
	cti_capture_consume(capture);
	return (data);
}

/// @brief Consume the captured output and compare it to @p expected.
/// @param capture Capture handle, NULL is accepted.
/// @param expected Exact output the code under test should have produced.
/// @return 0 when the output matches, -1 on mismatch, on error or when an
/// argument is NULL.
int ct_capture_compare(t_capture *capture, const char *expected)
{
	size_t expected_len;
	bool   equal;

	if (!capture || !expected || capture->pipe_read == -1)
		return (-1);
	cti_fflush_step(capture);
	if (!cti_capture_fill(capture))
		return (-1);
	expected_len = strlen(expected);
	equal = (capture->pending_len == expected_len &&
			 (capture->pending_len == 0 ||
			  memcmp(capture->pending, expected, capture->pending_len) == 0));
	cti_capture_consume(capture);
	return (equal ? 0 : -1);
}

/// @brief Inspect the captured output without consuming it.
/// @details Reads everything currently available on the capture pipe without
/// blocking and copies at most @p n bytes into @p buffer. The data is kept in
/// the capture, so it can be peeked again or consumed later by
/// ct_capture_get() or ct_capture_compare(). No NUL terminator is written:
/// embedded NUL bytes are copied as-is and the same output can be inspected
/// twice.
/// @param capture Capture handle, NULL is accepted.
/// @param n Maximum number of bytes to copy into @p buffer.
/// @param buffer Destination buffer of at least @p n bytes, NULL is accepted
/// when @p n is 0.
/// @return The number of bytes copied (0 when nothing was written), or -1 on
/// error, when @p capture is NULL, when it does not capture an output
/// descriptor, or when @p buffer is NULL while @p n is not 0.
ssize_t ct_capture_peekn(t_capture *capture, size_t n, char *buffer)
{
	size_t copied;

	if (!capture || capture->pipe_read == -1)
		return (-1);
	if (!buffer && n != 0)
		return (-1);
	cti_fflush_step(capture);
	if (!cti_capture_fill(capture))
		return (-1);
	copied = capture->pending_len < n ? capture->pending_len : n;
	if (copied)
		memcpy(buffer, capture->pending, copied);
	return ((ssize_t)copied);
}
