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

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "tc_modules.h"

static int	specific_return_check(void)
{
	return (42);
}

int	tcit_return_check(void)
{
	const int	expected_returns[] = {42, EXIT_FAILURE, EXIT_SUCCESS};
	t_list		*it;
	t_module	*t;
	int			i;

	tc_module_init(&t, "dummy", "dummy for tests");
	tc_module_taddf(t, dummy_success_test, "success_test");
	tc_module_taddf(t, dummy_fail_test, "fail_test");
	tc_module_taddf(t, specific_return_check, "specific_return_test");
	i = tc_module_run(t);
	if (i != EXIT_SUCCESS || t->tests_count != 3 || t->tests_passed != 1)
		return (tc_module_destroy(t), 2);
	it = t->tests_list;
	while (it)
	{
		if (((t_test *)it->data)->return_value != expected_returns[i++])
			return (tc_module_destroy(t), 3 + i - 1);
		it = it->next;
	}
	return (tc_module_destroy(t), EXIT_SUCCESS);
}
