/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 12:21:48 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/02 12:21:48 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fcntl.h"
#include <unistd.h>

#define CTESTS_INTERNAL
#include "tc_capture.h"
#include "tc_memory.h"
#include "tc_utils.h"

// Set non-blocking mode
static int __attribute__((unused))	tci_capture_nonblock(t_capture *c)
{
	int	f;

	f = fcntl(c->pipe_read, F_GETFL, 0);
	if (f == -1)
		return (-1);
	if (fcntl(c->pipe_read, F_SETFL, f | O_NONBLOCK) == -1)
		return (-1);
	return (0);
}

// to enable capture nonblock you can pop these lines at the back
// if (tci_capture_nonblock(capture) == -1)
// 	return (tc_capture_release(capture), false);
bool	tc_capture_init(int source_fd, t_capture *capture)
{
	int	pipe_fds[2];

	if (!capture || source_fd < 0)
		return (false);
	tc_memset(capture, 0, sizeof(*capture));
	capture->source_fd = source_fd;
	capture->original_fd = dup(source_fd);
	if (capture->original_fd == -1)
		return (false);
	if (pipe(pipe_fds) == -1)
		return (close(capture->original_fd), false);
	capture->pipe_read = pipe_fds[0];
	capture->pipe_write = pipe_fds[1];
	if (dup2(capture->pipe_write, source_fd) == -1)
		return (close(capture->original_fd), close(capture->pipe_read),
			close(capture->pipe_write), false);
	close(capture->pipe_write);
	capture->pipe_write = -1;
	return (true);
}

t_capture	*tc_capture_create(int fd)
{
	t_capture	*tmp;

	tmp = safealloc(sizeof(*tmp));
	if (!tmp)
		return (NULL);
	tc_memset(tmp, 0, sizeof(*tmp));
	if (tc_capture_init(fd, tmp) == false)
		return (tc_capture_destroy(tmp), NULL);
	return (tmp);
}
