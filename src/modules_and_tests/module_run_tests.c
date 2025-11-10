/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_run_tests.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 17:30:13 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/01 17:30:13 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdlib.h>

#define CTESTS_INTERNAL
#include "tc_memory.h"
#include "tc_modules.h"

static int	innner_run_test(t_test *test, t_module *m)
{
	if (FORK_TESTS)
		return (tci_tfork(test, m));
	test->pid = -1;
	test->return_value = test->test_func();
	return (test->return_value);
}

static void	setups(t_module *m)
{
	if (m->auto_cleanup == true)
		tci_set_memory_cleanup(true);
	if (m->setup_function)
		m->setup_function();
	return ;
}

static void	teardown(t_module *m)
{
	if (tci_get_memory_cleanup() == true)
	{
		do_cleanup(m->clean_up_function);
		tci_set_memory_cleanup(false);
	}
	if (m->teardown_function)
		m->teardown_function();
	return ;
}

int	tc_module_run_self_tests(t_module *module)
{
	t_list	*t_ls;
	int		ret;

	ret = 0;
	setups(module);
	t_ls = module->tests_list;
	while (t_ls)
	{
		tc_failloc_reset();
		if (innner_run_test(t_ls->data, module) != EXIT_SUCCESS)
			ret = EXIT_FAILURE;
		t_ls = t_ls->next;
	}
	teardown(module);
	return (ret);
}
