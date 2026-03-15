/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cm_alloc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 13:25:06 by bgoulard          #+#    #+#             */
/*   Updated: 2026/01/22 13:39:04 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CTESTS_INTERNAL_TESTS
#include "tc_modules.h"
#include "tc_memory.h"

t_module	*create_alloc_tests(void)
{
	t_module	*re;

	tc_module_init(&re, "alloc", "desc");
	if (!re)
		return (NULL);
	tc_module_taddf(re, tcit_cleanup, "auto clenaup test");
	tc_module_taddf(re, tcit_count, "allocations count");
	tc_module_taddf(re, tcit_fail, "failloc usage test");
	return (re);
}
