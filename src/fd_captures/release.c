/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   release.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 12:30:27 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/02 12:30:27 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#define CTESTS_INTERNAL
#include "tc_capture.h"

bool	tc_capture_release(t_capture *capture)
{
	bool	successful_reset_of_fd;

	if (!capture)
		return (false);
	successful_reset_of_fd = true;
	tci_fflush_step(capture);
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
