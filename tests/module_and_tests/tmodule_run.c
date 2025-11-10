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

#define CTESTS_INTERNAL_TESTS
#include "tc_modules.h"

int	tcit_run(void)
{
	t_module	*t;
	int			r;

	tc_module_init(&t, "test", "module for tests to run on");
	r = tc_module_run(NULL);
	if (r != EXIT_FAILURE)
		return (tc_module_destroy(t), EXIT_FAILURE);
	r = tc_module_run(t);
	if (r != EXIT_SUCCESS)
		return (tc_module_destroy(t), r);
	tc_module_taddf(t, dummy_success_test, "test1");
	tc_module_taddf(t, dummy_fail_test, "test2");
	r = tc_module_run(t);
	return (tc_module_destroy(t), r);
}
