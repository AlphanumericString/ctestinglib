/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_module_nb_tests.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:31:40 by bgoulard          #+#    #+#             */
/*   Updated: 2025/10/30 23:31:40 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "tc_modules.h"
#include "tc_memory.h"

// can we add tests?
// does the add_test_f work as intended?
// what about add_test?
static int	test_add_test(t_module *t)
{
	t_test	*tmp;
	int		pre_total;

	tmp = safealloc(sizeof(*tmp));
	if (!tmp)
		return (1);
	*tmp = (t_test){0};
	tmp->test_func = dummy_success_test;
	tmp->name = "";
	pre_total = t->tests_count;
	tc_module_tadd(t, tmp);
	if (t->tests_list->data != tmp || t->tests_count != pre_total + 1)
		return (1);
	return (EXIT_SUCCESS);
}

static int	test_add_test_f(t_module *t)
{
	tc_module_taddf(t, dummy_success_test, "ok1");
	tc_module_taddf(t, dummy_fail_test, "ko1");
	tc_module_taddf(t, dummy_success_test, "ok2");
	if (t->tests_count != 3)
		return (1);
	return (EXIT_SUCCESS);
}

int	tcit_nb_tests(void)
{
	t_module	*t;
	int			r;

	tc_module_init(&t, "test", "module for tests to run on");
	r = test_add_test_f(t);
	if (r)
		return (tc_module_destroy(t), r + 8);
	r = test_add_test(t);
	if (r)
		return (tc_module_destroy(t), r + 16);
	return (tc_module_destroy(t), EXIT_SUCCESS);
}
