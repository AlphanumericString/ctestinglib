/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dummy_modules.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 14:14:13 by bgoulard          #+#    #+#             */
/*   Updated: 2025/09/26 14:14:13 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define INTERNAL_TESTINGLIB
#define INTERNAL_TESTINGLIB_TESTS
#include "tests_fxtr.h"

t_module	*create_string_tests(void)
{
	t_module	*str;

	str = safealloc(sizeof(*str));
	if (!str)
		return (NULL);
	init_module(str, "str", "tests for dummy string module");
	add_test_f(str, dummy_success_test, "ss1");
	return (str);
}

t_module	*create_list_tests(void)
{
	t_module	*list;

	list = safealloc(sizeof(*list));
	if (!list)
		return (NULL);
	init_module(list, "list", "tests for dummy list module");
	add_test_f(list, dummy_fail_test, "lf1");
	add_test_f(list, dummy_success_test, "ls1");
	add_test_f(list, dummy_success_test, "ls2");
	add_test_f(list, dummy_success_test, "ls3");
	add_test_f(list, dummy_sigsegv, "lc1");
	return (list);
}
