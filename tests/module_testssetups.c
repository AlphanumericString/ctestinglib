/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_testssetups.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 21:02:56 by bgoulard          #+#    #+#             */
/*   Updated: 2025/10/31 21:02:56 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define INTERNAL_TESTINGLIB
#define INTERNAL_TESTINGLIB_TESTS
#include "tests_fxtr.h"

t_module	*create_tests_tests(void)
{
	t_module		*tests_tests;

	tests_tests = safealloc(sizeof(*tests_tests));
	if (!tests_tests)
		return (NULL);
	init_module(tests_tests, "Tests", "Check on handling tests functionalities");
	add_test_f(tests_tests, ttest_add, "adding tests to modules");
	add_test_f(tests_tests, ttest_crash_catcher, "test crash catcher");
	add_test_f(tests_tests, ttest_ordering, "tests ordering");
	add_test_f(tests_tests, ttest_return_check, "tests return value checking");
	return (tests_tests);
}
