/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_state.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 00:30:05 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/14 00:31:05 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CTESTS_INTERNAL
#include "tc_memory.h"

static bool	*tci_memclnup_state(void)
{
	static bool	state = false;

	return (&state);
}

void	tci_set_memory_cleanup(bool val)
{
	*tci_memclnup_state() = val;
}

bool	tci_get_memory_cleanup(void)
{
	return (*tci_memclnup_state());
}
