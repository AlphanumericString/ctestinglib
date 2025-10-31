/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_test_crash_catcher.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 21:05:08 by bgoulard          #+#    #+#             */
/*   Updated: 2025/10/31 21:05:08 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <signal.h>

#define INTERNAL_TESTINGLIB
#define INTERNAL_TESTINGLIB_TESTS
#include "tests_fxtr.h"

int	ttest_crash_catcher(void)
{
	t_module	*t;
	int			ret;

	t = safealloc(sizeof(*t));
	if (!t)
		return (EXIT_FAILURE);
	init_module(t, "dummy", "dummy for tests");
	add_test_f(t, dummy_sigsegv, "crash_test");
	ret = run_module(t);
	if (ret != EXIT_SUCCESS)
		return (module_destroy(t), 1);
	if (t->tests_passed != 0 || t->tests_count != 1)
		return (module_destroy(t), 2);
	if (((t_test *)t->tests_list->data)->return_value != (SIGSEGV + 128))
		return (module_destroy(t), 3);
	return (module_destroy(t), EXIT_SUCCESS);
}
