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

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "tc_modules.h"

int	tcit_crash_catcher(void)
{
	t_module	*t;
	int			ret;

	tc_module_init(&t, "dummy", "dummy for tests");
	tc_module_taddf(t, dummy_sigsegv, "crash_test");
	ret = tc_module_run(t);
	if (ret != EXIT_SUCCESS)
		return (tc_module_destroy(t), 1);
	if (t->tests_passed != 0 || t->tests_count != 1)
		return (tc_module_destroy(t), 2);
	if (((t_test *)t->tests_list->data)->return_value != (SIGSEGV + 128))
		return (tc_module_destroy(t), 3);
	return (tc_module_destroy(t), EXIT_SUCCESS);
}
