/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_test_order.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 21:33:40 by bgoulard          #+#    #+#             */
/*   Updated: 2025/10/31 21:33:40 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#define INTERNAL_TESTINGLIB
#define INTERNAL_TESTINGLIB_TESTS
#include "tests_fxtr.h"

static int	leaf_ordering_check(t_module *t)
{
	void		*expected_forder[] = {dummy_success_test, (void *)0xDEAF, (void *)0xCAFE,
		dummy_fail_test, (void *)0xDEAD};
	t_list		*leaf;
	size_t		i;

	i = 0;
	leaf = t->tests_list;
	while (leaf)
	{
		if ((void *)((t_test *)leaf->data)->test_func != expected_forder[i++])
			return (EXIT_FAILURE + i - 1);
		leaf = leaf->next;
	}
	return (i - t->tests_count);
}

static t_module	*local_dummy_module_create(void)
{
	t_module	*t;

	t = safealloc(sizeof(*t));
	if (!t)
		return (NULL);
	init_module(t, "dummy", "module created for testing purposes");
	add_test_f(t, dummy_success_test, "aaa");
	add_test_f(t, dummy_fail_test, "aba");
	add_test_f(t, (void *)0xDEAD, "baa");
	add_test_f(t, (void *)0xCAFE, "aab");
	add_test_f(t, (void *)0xDEAF, "aaaa");
	return (t);
}

int	ttest_ordering(void)
{
	t_module	*t;
	t_module	*sub;

	t = local_dummy_module_create();
	if (!t)
		return (EXIT_FAILURE);
	sort_recursive(t);
	if (leaf_ordering_check(t))
		return (module_destroy(t), 1);
	sub = create_string_tests();
	if (!sub)
		return (module_destroy(t), EXIT_FAILURE);
	add_submodule(t, sub);
	sort_recursive(t);
	if (leaf_ordering_check(sub))
		return (module_destroy(t), 2);
	return (module_destroy(t), EXIT_SUCCESS);
}

