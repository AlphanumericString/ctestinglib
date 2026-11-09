/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 13:18:17 by bgoulard          #+#    #+#             */
/*   Updated: 2026/01/22 15:15:34 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>
#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "tc_memory.h"
#include "tc_modules.h"
#include <stdlib.h>

// TODO: maybe move the m->auto_cleanup
int	tcit_cleanup(void)
{
	t_module	*tmp_dummy;

	tc_module_init(&tmp_dummy, "module_dummy", "dummy module to test cleanups");
	tc_module_taddf(tmp_dummy, dummy_alloc, "alloc");
	// tc_module_taddf(tmp_dummy, dummy_leak, "leak");
	tmp_dummy->auto_cleanup = true;
	tc_module_run(tmp_dummy);
	printf("\n\n\n");
	tc_module_display(tmp_dummy, TC_DISPLAY_DEFAULT);
	printf("\n\n\n");
	tc_module_destroy(tmp_dummy);
	return (EXIT_SUCCESS);
}

int	tcit_count(void)
{
	return (EXIT_SUCCESS);
}

int	tcit_fail(void)
{
	return (EXIT_SUCCESS);
}
