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

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "tc_modules.h"

static int	ttest_add_param_checks(void)
{
	tc_module_tadd(NULL, (t_test *)0xDEADBEEF);
	tc_module_tadd((t_module *)0xDEADBEEF, NULL);
	return (EXIT_SUCCESS);
}

static int	ttest_addf(t_module *t)
{
	tc_module_taddf(NULL, dummy_success_test, "ok1");
	tc_module_taddf(t, NULL, "aaaa");
	tc_module_taddf(t, dummy_success_test, NULL);
	if (t->tests_count != 0)
		return (2);
	return (EXIT_SUCCESS);
}

int	tcit_tadd(void)
{
	const char	*names[] = {"test1", "test2", "test3"};
	t_module	*t;

	tc_module_init(&t, "dummy", "dummy for tests");
	ttest_add_param_checks();
	ttest_addf(t);
	tc_module_taddf(t, dummy_success_test, names[0]);
	if (t->tests_count != 1 && t->tests_list == NULL)
		return (tc_module_destroy(t), 1);
	tc_module_taddf(t, dummy_success_test, names[1]);
	tc_module_taddf(t, dummy_success_test, names[2]);
	if (t->tests_count != 3 && t->tests_list == NULL)
		return (tc_module_destroy(t), 2);
	if (((t_test *)t->tests_list->data)->name != names[2]
		|| ((t_test *)t->tests_list->next->data)->name != names[1]
		|| ((t_test *)t->tests_list->next->next->data)->name != names[0])
		return (tc_module_destroy(t), 3);
	return (tc_module_destroy(t), EXIT_SUCCESS);
}
