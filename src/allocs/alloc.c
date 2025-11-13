/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 23:58:07 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/14 00:31:08 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CTESTS_INTERNAL
#include "tc_memory.h"

extern void	*__libc_malloc(size_t size);
extern void	__libc_free(void *p);

// This is a by-pass around the malloc wrapper for the development of the
// testing library.
void	*safealloc(size_t size)
{
	return (__libc_malloc(size));
}

void	safefree(void *p)
{
	return (__libc_free(p));
}
