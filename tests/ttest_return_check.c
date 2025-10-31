/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_test_return_check.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 21:35:34 by bgoulard          #+#    #+#             */
/*   Updated: 2025/10/31 21:35:34 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#define INTERNAL_TESTINGLIB
#define INTERNAL_TESTINGLIB_TESTS
#include "tests_fxtr.h"

static int	specific_return_check(void)
{
	return (42);
}

int	ttest_return_check(void)
{
	const int	expected_returns[] = { 42, EXIT_FAILURE, EXIT_SUCCESS};
	t_list		*it;
	t_module	*t;
	int			i;

	t = safealloc(sizeof(*t));
	if (!t)
		return (EXIT_FAILURE);
	init_module(t, "dummy", "dummy for tests");
	add_test_f(t, dummy_success_test, "success_test");
	add_test_f(t, dummy_fail_test, "fail_test");
	add_test_f(t, specific_return_check, "specific_return_test");
	i = run_module(t);
	if (i != EXIT_SUCCESS || t->tests_count != 3 || t->tests_passed != 1)
		return (module_destroy(t), 2);
	it = t->tests_list;
	while (it)
	{
		if (((t_test *)it->data)->return_value != expected_returns[i++])
			return (module_destroy(t), 3 + i - 1);
		it = it->next;
	}
	return (module_destroy(t), EXIT_SUCCESS);
}
