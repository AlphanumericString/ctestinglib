/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmodule_run.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 22:15:05 by bgoulard          #+#    #+#             */
/*   Updated: 2025/10/31 22:15:05 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#define INTERNAL_TESTINGLIB
#define INTERNAL_TESTINGLIB_TESTS

#include "tests_fxtr.h"

int	tmodule_run(void)
{
	t_module	*t;
	int			r;

	t = safealloc(sizeof(*t));
	if (!t)
		return (EXIT_FAILURE);
	init_module(t, "test", "module for tests to run on");
	r = run_module(NULL);
	if (r != EXIT_FAILURE)
		return (module_destroy(t), EXIT_FAILURE);
	r = run_module(t);
	if (r != EXIT_SUCCESS)
		return (module_destroy(t), r);
	add_test_f(t, dummy_success_test, "test1");
	add_test_f(t, dummy_fail_test, "test2");
	r = run_module(t);
	return (module_destroy(t), r);
}
