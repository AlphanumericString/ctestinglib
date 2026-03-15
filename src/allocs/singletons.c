/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singletons.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 22:57:05 by bgoulard          #+#    #+#             */
/*   Updated: 2026/01/13 22:57:05 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CTESTS_INTERNAL
#include "tc_memory.h"

int64_t	*tci_failloc_get(void)
{
	static int64_t	nb_before_fail = INT64_MAX;

	return (&nb_before_fail);
}

size_t	*tci_alloc_get(void)
{
	static size_t	nb_allocs = 0;

	return (&nb_allocs);
}
