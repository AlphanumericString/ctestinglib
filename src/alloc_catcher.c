/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_catcher.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 23:58:07 by bgoulard          #+#    #+#             */
/*   Updated: 2025/08/14 23:58:07 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define INTERNAL_TESTINGLIB
#include "tests_fxtr.h"
#include <stdlib.h>

extern void	*__libc_malloc(size_t size);

// This function is a wrapper around the standard malloc function.
// This is designed to tests the behavior of functions in case of memory
// allocation failure.
// DO NOT USE TO DEV THE TESTING LIB!!!
void	*malloc(size_t size)
{
	return (__libc_malloc(size));
}

// This is a by-pass around the malloc wrapper for the development of the
// testing library.
void	*safealloc(size_t size)
{
	return (__libc_malloc(size));
}
