#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define CTESTS_INTERNAL
#include "ct_internal.h"

static void append_string(char **src, const char *append)
{
	char  *ret;
	size_t len;

	len = 0;
	if (src && *src)
		len += strlen(*src);
	if (append)
		len += strlen(append);
	ret = safealloc(len + 1);
	if (!ret)
		return (safefree(*src), *src = NULL, (void)0);
	ret[len] = 0;
	if (src && *src)
	{
		memcpy(ret, *src, strlen(*src));
		safefree(*src);
	}
	if (append)
		memcpy(ret + (len - strlen(append)), append, strlen(append));
	*src = ret;
}

char *ct_capture_get(t_capture *c_fd)
{
	char  buffer[CT_RBUFFSIZE + 1];
	int	  n;
	char *ret;

	if (!c_fd || c_fd->pipe_read == -1)
		return (NULL);
	ret = NULL;
	n = CT_RBUFFSIZE;
	while (n == CT_RBUFFSIZE)
	{
		n = read(c_fd->pipe_read, buffer, CT_RBUFFSIZE);
		if (n < 0)
			return (safefree(ret), NULL);
		buffer[n] = '\0';
		append_string(&ret, buffer);
	}
	return (ret);
}

int ct_capture_compare(t_capture *c_fd, const char *exp)
{
	char		 buffer[CT_RBUFFSIZE + 1];
	ssize_t		 n;
	const size_t exp_len = strlen(exp);
	ssize_t		 total;

	if (!c_fd || !exp || exp_len == 0 || c_fd->pipe_read == -1)
		return (-1);
	cti_fflush_step(c_fd);
	total = 0;
	n = CT_RBUFFSIZE;
	while (n == CT_RBUFFSIZE)
	{
		n = read(c_fd->pipe_read, buffer, CT_RBUFFSIZE);
		if (n < 0)
			return (-1);
		total += n;
		buffer[n] = 0;
		if (total == (ssize_t)exp_len)
			break;
		if (total >= (ssize_t)exp_len || memcmp(buffer, exp, n) != 0)
			return (-1);
		exp += n;
	}
	return (memcmp(buffer, exp, n));
}
