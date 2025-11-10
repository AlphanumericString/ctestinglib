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

#include <stdio.h>
#define CTESTS_INTERNAL
#include "tc_modules.h"
#include "tc_utils.h"

static void	module_display_tests_infos(const t_module *m, t_display_mode md)
{
	int	test_total;
	int	test_passed;

	test_total = tci_collect_sbm_t_total(m);
	test_passed = tci_collect_sbm_t_success(m);
	if ((md & TC_DSBM_GROUPING_MASK) == TC_DSBM_SPLIT)
	{
		test_total = m->tests_count;
		test_passed = m->tests_passed;
	}
	if ((md & TC_DSBM_SUMMARY_MASK) == TC_DSBM_SUMNB)
		tci_mdisplay_sum_nb(test_total, test_passed, md);
	if ((md & TC_DSBM_SUMMARY_MASK) == TC_DSBM_SUMPRECENT)
		tci_mdisplay_sum_percent(test_total, test_passed, md);
	tci_mdisplay_sbm(m->submodules_list, md);
}

static void	module_print(const t_module *m, t_display_mode md, int depth, bool fwrd)
{
	if ((md & TC_FMT_MASK) == TC_FMT_SILENT || (md & TC_DM_SUMMARY_MASK) == TC_DM_SILENT)
		return ;
	tci_print_charn(depth, '\t');
	tci_print_string(m->name);
	if (!fwrd)
		return ;
	tci_print_string("\t");
	tci_print_string(m->description);
}

void	tci_mdisplay_fwrd(const t_module *m, t_display_mode md, int depth)
{
	if ((md & TC_FMT_MASK) == TC_FMT_SILENT
		|| (md & TC_DM_SUMMARY_MASK) == TC_DM_SILENT)
		return ;
	module_print(m, md, depth, true);
	tci_print_string("\n");
}

void	tci_mdisplay_bwrd(const t_module *m, t_display_mode md, int depth)
{
	if ((md & TC_FMT_MASK) == TC_FMT_SILENT
		|| (md & TC_DM_SUMMARY_MASK) == TC_DM_SILENT)
		return ;
	if ((md & TC_DM_SUMMARY_MASK) == TC_DM_SUMARY
		&& !((md & TC_DSBM_GROUPING_MASK) == TC_DSBM_MERGED)
		&& m->tests_count == 0)
		return ;
	module_print(m, md, depth, false);
	if ((md & TC_DM_SUMMARY_MASK) == TC_DM_SILENT
		|| ((md & TC_DSBM_GROUPING_MASK) == TC_DSBM_SPLIT
		&& m->submodules_list == NULL))
		return ((void)tci_print_string("\n"));
	tci_print_string("\t");
	module_display_tests_infos(m, md);
	tci_print_string("\n");
}
