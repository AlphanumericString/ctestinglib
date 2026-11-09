/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmodule_errors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 15:30:00 by bgoulard          #+#    #+#             */
/*   Updated: 2026/01/22 15:30:00 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "tc_modules.h"

int	tcit_module_errors(void)
{
	t_module	*m;
	t_module	*sub;
	t_test		t;

	tc_module_sort(NULL);
	tc_module_destroy(NULL);
	tc_module_init(NULL, "x", "y");
	m = NULL;
	tc_module_init(&m, NULL, "y");
	if (m != NULL)
		return (1);
	tc_module_init(&m, "m", "d");
	t = (t_test){0};
	sub = create_quiet_dummy();
	if (tc_module_add_sub(NULL, sub) != false)
		return (tc_module_destroy(sub), tc_module_destroy(m), 2);
	tc_module_destroy(sub);
	if (tc_module_add_sub(m, NULL) != false)
		return (tc_module_destroy(m), 3);
	if (tc_module_taddf(NULL, dummy_success_test, "x") != false)
		return (tc_module_destroy(m), 4);
	if (tc_module_taddf(m, NULL, "x") != false)
		return (tc_module_destroy(m), 5);
	if (tc_module_taddf(m, dummy_success_test, NULL) != false)
		return (tc_module_destroy(m), 6);
	if (tc_module_tadd(NULL, &t) != false)
		return (tc_module_destroy(m), 7);
	if (tc_module_tadd(m, NULL) != false)
		return (tc_module_destroy(m), 8);
	tc_module_destroy(m);
	return (EXIT_SUCCESS);
}
