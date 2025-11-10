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

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "tc_modules.h"

int	tcit_nb_passed(void)
{
	t_module	*t;

	tc_module_init(&t, "test", "module for tests to run on");
	tc_module_taddf(t, dummy_success_test, "ok1");
	tc_module_run(t);
	if (t->tests_passed != 1)
		return (tc_module_destroy(t), 1);
	tc_module_taddf(t, dummy_fail_test, "ko1");
	tc_module_run(t);
	if (t->tests_passed != 1 && t->tests_count != 2)
		return (tc_module_destroy(t), 2);
	tc_module_taddf(t, dummy_sigsegv, "crash1");
	tc_module_run(t);
	if (t->tests_passed != 1 && t->tests_count != 3)
		return (tc_module_destroy(t), 3);
	return (tc_module_destroy(t), EXIT_SUCCESS);
}
