/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_display_txt_inner.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:27:40 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/13 01:29:41 by bgoulard         ###   ########.fr       */
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
	if ((md & TC_DSBM_GRPMSK) == TC_DSBM_SEP)
	{
		test_total = m->tests_count;
		test_passed = m->tests_passed;
	}
	if ((md & TC_DM_SUMMSK) == TC_DM_SUMNBS)
		tci_mdisplay_sum_nb(test_total, test_passed, md);
	if ((md & TC_DM_SUMMSK) == TC_DM_SUMPCT)
		tci_mdisplay_sum_percent(test_total, test_passed, md);
	if ((md & TC_DM_SUMMSK) != TC_DM_SUMSILENT && m->submodules_list)
		tci_print_string("\t");
	tci_mdisplay_sbm(m->submodules_list, md);
}

static void	module_print(const t_module *m, int depth, bool fwrd)
{
	tci_print_charn(depth, '\t');
	tci_print_string(m->name);
	if (!fwrd)
		return ;
	tci_print_string("\t");
	tci_print_string(m->description);
}

static void	tci_mdisplay_fwrd(const t_module *m, t_display_mode md, int depth)
{
	if ((md & TC_DM_SUMMSK) == TC_DM_SUMSILENT)
		return ;
	module_print(m, depth, true);
	tci_print_string("\n");
}

static void	tci_mdisplay_bwrd(const t_module *m, t_display_mode md, int depth)
{
	if ((md & TC_DM_SUMMSK) == TC_DM_SUMSILENT)
		return ;
	if ((md & TC_DM_SUMMSK) == TC_DM_SUMNBS
		&& !((md & TC_DSBM_GRPMSK) == TC_DSBM_MRG)
		&& m->tests_count == 0)
		return ;
	module_print(m, depth, false);
	if ((md & TC_DM_SUMMSK) == TC_DM_SUMSILENT
		|| ((md & TC_DSBM_GRPMSK) == TC_DSBM_SEP && m->submodules_list == NULL))
		return ((void)tci_print_string("\n"));
	tci_print_string("\t");
	module_display_tests_infos(m, md);
}

void	tci_display_results_txt(const t_module *m, t_display_mode md, int depth)
{
	const int	dp = depth;
	t_list		*md_l;

	md_l = m->submodules_list;
	tci_mdisplay_fwrd(m, md, depth);
	while (md_l)
	{
		tci_display_results_txt(md_l->data, md, dp + 1);
		md_l = md_l->next;
	}
	tci_mdisplay_tests(m, md, depth);
	tci_mdisplay_bwrd(m, md, depth);
}
