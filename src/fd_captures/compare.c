/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compare.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 12:33:40 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/02 12:33:40 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tc_memory.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define CTESTS_INTERNAL
#include "tc_utils.h"
#include "tc_capture.h"

static void	append_string(char **src, const char *append)
{
	char	*ret;
	size_t	len;

	len = 0;
	if (src && *src)
		len += tc_strlen(*src);
	if (append)
		len += tc_strlen(append);
	ret = safealloc(len + 1);
	if (!ret)
		return (safefree(*src), *src = NULL, (void)0);
	ret[len] = 0;
	if (src && *src)
	{
		tc_memcpy(ret, *src, tc_strlen(*src));
		safefree(*src);
	}
	if (append)
		tc_memcpy(ret + (len - tc_strlen(append)), append, tc_strlen(append));
	*src = ret;
}

char	*tc_capture_get(t_capture *c_fd)
{
	char	buffer[TC_RBUFFSIZE + 1];
	int		n;
	char	*ret;

	if (!c_fd || c_fd->pipe_read == -1)
		return (NULL);
	ret = NULL;
	n = TC_RBUFFSIZE;
	while (n == TC_RBUFFSIZE)
	{
		n = read(c_fd->pipe_read, buffer, TC_RBUFFSIZE);
		if (n < 0)
			return (safefree(ret), NULL);
		buffer[n] = '\0';
		append_string(&ret, buffer);
	}
	return (ret);
}

int	tc_capture_compare(t_capture *c_fd, const char *exp)
{
	char			buffer[TC_RBUFFSIZE + 1];
	ssize_t			n;
	const size_t	exp_len = tc_strlen(exp);
	ssize_t			total;

	if (!c_fd || !exp || exp_len == 0 || c_fd->pipe_read == -1)
		return (-1);
	tci_fflush_step(c_fd);
	total = 0;
	n = TC_RBUFFSIZE;
	while (n == TC_RBUFFSIZE)
	{
		n = read(c_fd->pipe_read, buffer, TC_RBUFFSIZE);
		if (n < 0)
			return (-1);
		total += n;
		buffer[n] = 0;
		if (total == (ssize_t)exp_len)
			break ;
		if (total >= (ssize_t)exp_len || tc_memcmp(buffer, exp, n) != 0)
			return (-1);
		exp += n;
	}
	return (tc_memcmp(buffer, exp, n));
}
