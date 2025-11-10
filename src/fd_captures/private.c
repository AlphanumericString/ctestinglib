/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   private.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 12:25:17 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/02 12:25:17 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>

#define CTESTS_INTERNAL
#include "tc_capture.h"

void	tci_fflush_step(t_capture *c)
{
	if (c->source_fd == STDOUT_FILENO)
		fflush(stdout);
	else if (c->source_fd == STDERR_FILENO)
		fflush(stderr);
}
