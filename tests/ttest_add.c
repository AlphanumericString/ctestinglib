/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_test_add.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 20:42:59 by bgoulard          #+#    #+#             */
/*   Updated: 2025/10/31 20:42:59 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#define INTERNAL_TESTINGLIB
#define INTERNAL_TESTINGLIB_TESTS
#include "tests_fxtr.h"

static int	ttest_add_param_checks(void)
{
	add_test(NULL, (t_test*)0xDEADBEEF);
	add_test((t_module*)0xDEADBEEF, NULL);
	return (EXIT_SUCCESS);
}

static int	ttest_addf(t_module *t)
{
	add_test_f(NULL, dummy_success_test, "ok1");
	add_test_f(t, NULL, "aaaa");
	add_test_f(t, dummy_success_test, NULL);
	if (t->tests_count != 0)
		return (2);
	return (EXIT_SUCCESS);
}

int	ttest_add(void)
{
	const char	*names[] = {"test1", "test2", "test3"};
	t_module	*t;

	t = safealloc(sizeof(*t));
	if (!t)
		return (EXIT_FAILURE);
	init_module(t, "dummy", "dummy for tests");
	ttest_add_param_checks();
	ttest_addf(t);
	add_test_f(t, dummy_success_test, names[0]);
	if (t->tests_count != 1 && t->tests_list == NULL)
		return (module_destroy(t), 1);
	add_test_f(t, dummy_success_test, names[1]);
	add_test_f(t, dummy_success_test, names[2]);
	if (t->tests_count != 3 && t->tests_list == NULL)
		return (module_destroy(t), 2);
	if (((t_test *)t->tests_list->data)->name != names[2]
		|| ((t_test *)t->tests_list->next->data)->name != names[1]
		|| ((t_test *)t->tests_list->next->next->data)->name != names[0])
		return (module_destroy(t), 3);
	return (module_destroy(t), EXIT_SUCCESS);
}
