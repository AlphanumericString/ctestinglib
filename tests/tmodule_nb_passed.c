/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_module_nb_passed.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 04:44:35 by bgoulard          #+#    #+#             */
/*   Updated: 2025/10/29 04:44:35 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#define INTERNAL_TESTINGLIB
#define INTERNAL_TESTINGLIB_TESTS
#include "tests_fxtr.h"

int	tmodule_nb_passed(void)
{
	t_module	*t;

	t = safealloc(sizeof(*t));
	if (!t)
		return (EXIT_FAILURE);
	init_module(t, "test", "module for tests to run on");
	add_test_f(t, dummy_success_test, "ok1");
	run_module(t);
	if (t->tests_passed != 1)
		return (module_destroy(t), 1);
	add_test_f(t, dummy_fail_test, "ko1");
	run_module(t);
	if (t->tests_passed != 1 && t->tests_count != 2)
		return (module_destroy(t), 2);
	add_test_f(t, dummy_sigsegv, "crash1");
	run_module(t);
	if (t->tests_passed != 1 && t->tests_count != 3)
		return (module_destroy(t), 3);
	return (module_destroy(t), EXIT_SUCCESS);
}
