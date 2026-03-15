/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   failloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 17:10:26 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/14 00:31:11 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CTESTS_INTERNAL
#include "tc_memory.h"

// public version of tci_alloc_get, remoes the possibility of the user changing
// the values
size_t	tc_alloc_getcount(void)
{
	return (*tci_alloc_get());
}

void	tc_failloc_set(int nb)
{
	(*tci_failloc_get()) = (int64_t)nb;
}

void	tc_failloc_reset(void)
{
	(*tci_failloc_get()) = INT64_MAX;
}

// This function is a wrapper around the standard malloc function.
// This is designed to tests the behavior of functions in case of memory
// allocation failure.
// DO NOT USE TO DEV THE TESTING LIB!!!
void	*malloc(size_t size)
{
	void	*p;

	if (*tci_failloc_get() <= 0)
		return (NULL);
	(*tci_alloc_get())++;
	(*tci_failloc_get())--;
	p = safealloc(size);
	if (tci_get_memory_cleanup() == true)
		push_to_cleanup(p);
	return (p);
}

void	tc_auto_call(void (*f)(void))
{
	const size_t	current_ac = tc_alloc_getcount();
	size_t			after_count;

	f();
	after_count = tc_alloc_getcount();
	while (after_count != current_ac)
	{
		tc_failloc_set(after_count - current_ac);
		f();
		after_count--;
	}
}
