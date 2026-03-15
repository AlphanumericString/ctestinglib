/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dummy_tests.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 14:12:03 by bgoulard          #+#    #+#             */
/*   Updated: 2026/01/22 14:50:24 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdlib.h>

#define CTESTS_INTERNAL_TESTS
#include "tc_modules.h"

#include "tc_capture.h"

// smol populate
int	dummy_fail_test(void)
{
	return (EXIT_FAILURE);
}

int	dummy_success_test(void)
{
	return (EXIT_SUCCESS);
}

int	dummy_sigsegv(void)
{
	raise(SIGSEGV);
	return (EXIT_FAILURE);
}

int	dummy_capture_stdin(void)
{
	t_capture	*cap;
	int			ret;

	ret = EXIT_SUCCESS;
	cap = tc_capture_create(STDOUT_FILENO);
	if (!cap)
		return (EXIT_FAILURE);
	write(STDOUT_FILENO, "test", 4);
	if (tc_capture_compare(cap, "test") != 0)
		ret = EXIT_FAILURE;
	return (tc_capture_destroy(cap), ret);
}

int dummy_alloc(void)
{
	void *adress;

	adress = malloc(42);
	if (adress)
		return (free(adress), EXIT_SUCCESS);
	return (EXIT_FAILURE);
}
