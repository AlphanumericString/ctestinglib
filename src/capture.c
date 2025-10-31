/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   capture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 22:32:57 by bgoulard          #+#    #+#             */
/*   Updated: 2025/10/31 22:32:57 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define INTERNAL_TESTINGLIB
#include "memutils.h"
#include "tests_fxtr.h"

t_capture	*create_capture(int fd)
{
	t_capture	*tmp;

	tmp = safealloc(sizeof(*tmp));
	if (!tmp)
		return (NULL);
	self_memset(tmp, 0, sizeof(*tmp));
	if (capture_init(fd, tmp) == false)
		return (capture_destroy(tmp), NULL);
	return (tmp);
}

static int	__attribute__((unused)) capture_nonblock(t_capture *c)
{
	int f;

	// Set non-blocking mode
	f = fcntl(c->pipe_read, F_GETFL, 0);
	if (f == -1)
		return (perror("Failed to get pipe flags"), -1);
	if (fcntl(c->pipe_read, F_SETFL, f | O_NONBLOCK) == -1)
		return (perror("Failed to set non-blocking mode"), -1);
	return (0);
}

bool capture_init(int source_fd, t_capture *capture) {
	int			pipe_fds[2];

	if (!capture || source_fd < 0)
		return (false);
	self_memset(capture, 0, sizeof(*capture));
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

static void _fflush_step(t_capture *c)
{
	if (c->source_fd == STDOUT_FILENO)
		fflush(stdout);
	else if (c->source_fd == STDERR_FILENO)
		fflush(stderr);
}

#define BUFFER_SIZE 4096
int capture_compare(t_capture *capture, const char *expected) {
	char			buffer[BUFFER_SIZE + 1];
	ssize_t			n;
	const size_t	expected_len = strlen(expected);
	ssize_t			total;

	if (!capture || !expected || expected_len == 0)
		return (-1);
	_fflush_step(capture);
	total = 0;
	n = BUFFER_SIZE;
	while (n == BUFFER_SIZE)
	{
		n = read(capture->pipe_read, buffer, BUFFER_SIZE);
		if (n < 0)
			return (-1);
		total += n;
		buffer[n] = 0;
		if (total == (ssize_t)expected_len)
			break ;
		if (total >= (ssize_t)expected_len || memcmp(buffer, expected, n) != 0)
			return (-1);
		expected += n;
	}
	return (memcmp(buffer, expected, n));
}

bool capture_release(t_capture *capture)
{
	bool successful_reset_of_fd;

	if (!capture)
		return (false);
	successful_reset_of_fd = true;
	_fflush_step(capture);
	if (capture->original_fd != -1)
	{
		if (dup2(capture->original_fd, capture->source_fd) == -1)
			successful_reset_of_fd = false;
		close(capture->original_fd);
	}
	if (capture->pipe_read != -1)
		close(capture->pipe_read);
	if (capture->pipe_write != -1)
		close(capture->pipe_write);
	capture->original_fd = -1;
	capture->pipe_read = -1;
	capture->pipe_write = -1;
	return (successful_reset_of_fd);
}

void	capture_destroy(t_capture *c)
{
	if (!c)
		return ;
	capture_release(c);
	free(c);
}
