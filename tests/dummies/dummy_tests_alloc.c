/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dummy_tests_alloc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 16:13:47 by bgoulard          #+#    #+#             */
/*   Updated: 2026/04/05 16:13:47 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/cdefs.h>

#define CTESTS_INTERNAL_TESTS
#include "tc_modules.h"

int	dummy_alloc(void)
{
	void	*adress;

	adress = malloc(42);
	if (adress)
		return (free(adress), EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int	dummy_leak(void)
{
	void __attribute_maybe_unused__	*adress;

	adress = malloc(42);
	return (EXIT_SUCCESS);
}
