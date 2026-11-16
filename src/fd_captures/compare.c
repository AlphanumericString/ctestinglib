#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define CTESTS_INTERNAL
#include "ct_internal.h"

// Read everything currently available on the capture pipe. The read end is
// non blocking, so a short read means "nothing else was written yet", not an
// error.
static char *cti_capture_read(t_capture *capture, size_t *len)
{
	char	chunk[CT_RBUFFSIZE];
	char   *data;
	char   *grown;
	size_t	capacity;
	size_t	needed;
	size_t	new_capacity;
	size_t	got;
	ssize_t n;

	data = NULL;
	capacity = 0;
	*len = 0;
	for (;;)
	{
		n = read(capture->pipe_read, chunk, sizeof(chunk));
		if (n < 0)
		{
			if (errno == EINTR)
				continue;
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break;
			return (safefree(data), NULL);
		}
		if (n == 0)
			break;
		got = (size_t)n;
		if (*len > SIZE_MAX - got - 1)
			return (safefree(data), NULL);
		needed = *len + got + 1;
		if (needed > capacity)
		{
			new_capacity = capacity ? capacity : CT_RBUFFSIZE + 1;
			while (new_capacity < needed)
			{
				if (new_capacity > SIZE_MAX / 2)
					return (safefree(data), NULL);
				new_capacity *= 2;
			}
			grown = safealloc(new_capacity);
			if (!grown)
				return (safefree(data), NULL);
			if (data)
			{
				memcpy(grown, data, *len);
				safefree(data);
			}
			data = grown;
			capacity = new_capacity;
		}
		memcpy(data + *len, chunk, got);
		*len += got;
	}
	if (!data)
	{
		data = safealloc(1);
		if (!data)
			return (NULL);
	}
	data[*len] = '\0';
	return (data);
}

char *ct_capture_get(t_capture *capture)
{
	size_t len;

	if (!capture || capture->pipe_read == -1)
		return (NULL);
	cti_fflush_step(capture);
	return (cti_capture_read(capture, &len));
}

int ct_capture_compare(t_capture *capture, const char *expected)
{
	char  *actual;
	size_t actual_len;
	size_t expected_len;
	bool   equal;

	if (!capture || !expected || capture->pipe_read == -1)
		return (-1);
	cti_fflush_step(capture);
	actual = cti_capture_read(capture, &actual_len);
	if (!actual)
		return (-1);
	expected_len = strlen(expected);
	equal = (actual_len == expected_len &&
			 memcmp(actual, expected, actual_len) == 0);
	safefree(actual);
	return (equal ? 0 : -1);
}
