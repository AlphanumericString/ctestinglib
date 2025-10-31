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
	if (md & TM_SBM_SPLIT)
	{
		test_total = m->tests_count;
		test_passed = m->tests_passed;
	}
	module_sum_nb(test_total, test_passed, md);
	if (md & TM_SUM_ALL)
		print_string(" ");
	module_sum_percent(test_total, test_passed, md);
	module_sbm(m->submodules_list, md);
}

static void	module_print(t_module *m, t_display_mode md, int depth)
{
	if (md & TM_NONE && !((md & TM_PRINT_LAST) && !m->parent))
		return ;
	print_padding(depth);
	if (md & TM_INF_NAME)
		print_string(m->name);
	if (md & TM_INF_ALL)
		print_string("\t");
	if (md & TM_INF_DESC)
		print_string(m->description);
}

void	module_display_fwrd(t_module *m, t_display_mode md, int depth)
{
	if ((md & TM_NONE && !((md & TM_PRINT_LAST) && !m->parent))
		|| (md & TM_INF_ALL) == 0)
		return ;
	module_print(m, md, depth);
	if (md & TM_INF_ALL)
		print_string("\n");
}

void	module_display_bwrd(t_module *m, t_display_mode md, int depth)
{
	if (md & TM_NONE && (!((md & TM_PRINT_LAST) && !m->parent)
		|| (md & TM_MASK) == 0
		|| (md & TM_SBM_SPLIT && !m->submodules_list)))
		return ;
	if (md & TM_INF_ALL && !(md & TM_PRINT_SBM)
		&& m->tests_passed - m->tests_count == 0)
		return ;
	module_print(m, md & (~TM_INF_DESC), depth);
	if ((md & (TM_SUM_ALL | TM_SBM_SPLIT)) == 0)
		return ((void)print_string("\n"));
	if (md & TM_INF_ALL && md & TM_SUM_ALL && !(md & TM_NONE))
		print_string("\t");
	module_display_tests_infos(m, md);
	print_string("\n");
}
