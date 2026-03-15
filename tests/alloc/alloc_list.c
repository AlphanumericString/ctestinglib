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
#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "tc_memory.h"
#include "tc_modules.h"
#include <stdlib.h>

// TODO: maybe move the m->auto_cleanup
int tcit_cleanup(void)
{
	t_module	*tmp_dummy;

	tc_module_init(&tmp_dummy, "module_dummy", "dummy module to test cleanups");
	tc_module_taddf(tmp_dummy, dummy_alloc, "alloc");
	tmp_dummy->auto_cleanup = true;
	return (EXIT_SUCCESS);
}

int tcit_count(void)
{
	return (EXIT_SUCCESS);
}

int tcit_fail(void)
{
	return (EXIT_SUCCESS);
}
