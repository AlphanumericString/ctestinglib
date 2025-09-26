/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_display.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:27:40 by bgoulard          #+#    #+#             */
/*   Updated: 2025/08/08 18:27:40 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define INTERNAL_TESTINGLIB
#include "tests_fxtr.h"

static void	module_display_tests_infos(t_module *m, t_display_mode md)
{
	int			test_total;
	int			test_passed;

	test_total = _collect_sbm_t_total(m);
	test_passed = _collect_sbm_t_success(m);
	if (md & MODULE_PRINT_SBM_SPLIT)
	{
		test_total = m->tests_count;
		test_passed = m->tests_passed;
	}
	module_sum_nb(test_total, test_passed, md);
	if (md & MODULE_SUM_ALL)
		print_string(" ");
	module_sum_percent(test_total, test_passed, md);
	module_sbm(m->submodules_list, md);
}

static void	module_print(t_module *m, t_display_mode md, int depth)
{
	if (md & MODULE_PRINT_NONE && !((md & MODULE_PRINT_LAST) && !m->parent))
		return ;
	print_padding(depth);
	if (md & MODULE_INF_NAME)
		print_string(m->name);
	if (md & MODULE_INF_ALL)
		print_string("\t");
	if (md & MODULE_INF_DESC)
		print_string(m->description);
}

void	module_display_fwrd(t_module *m, t_display_mode md, int depth)
{
	if (md & MODULE_PRINT_NONE && !((md & MODULE_PRINT_LAST) && !m->parent))
		return ;
	module_print(m, md, depth);
	if (md & MODULE_INF_ALL)
		print_string("\n");
}

void	module_display_bwrd(t_module *m, t_display_mode md, int depth)
{
	if (md & MODULE_PRINT_NONE && !((md & MODULE_PRINT_LAST) && !m->parent))
		return ;
	module_print(m, md & (~MODULE_INF_DESC), depth);
	if ((md & MODULE_SUM_ALL) == 0)
		return ((void)print_string("\n"));
	if (md & MODULE_INF_ALL && md & MODULE_SUM_ALL && !(md & MODULE_PRINT_NONE))
		print_string("\t");
	module_display_tests_infos(m, md);
	print_string("\n");
}
