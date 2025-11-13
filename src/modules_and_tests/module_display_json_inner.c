/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_display_json_inner.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:27:40 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/14 00:31:49 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
}

static void	loc_md_json_fwrd(const t_module *m, t_display_mode md)
{
	if ((md & TC_DM_SUMMSK) == TC_DM_SUMSILENT)
		return ;
	tci_print_string("{\"module_name\":\"");
	tci_print_string(m->name);
	tci_print_string("\",\"description\":\"");
	tci_print_string(m->description);
	tci_print_string("\",");
}

static void	loc_md_json_bwrd(const t_module *m, t_display_mode md)
{
	module_display_tests_infos(m, md);
	tci_print_string("}");
}

void	tci_display_results_json(const t_module *m, t_display_mode md)
{
	t_list		*md_l;

	md_l = m->submodules_list;
	loc_md_json_fwrd(m, md);
	tci_mdisplay_tests(m, md, 0);
	tci_print_string(",");
	tci_print_string("\"submodules\":[");
	while (md_l)
	{
		tci_display_results_json(md_l->data, md);
		if (md_l->next)
			tci_print_string(",");
		md_l = md_l->next;
	}
	tci_print_string("],");
	loc_md_json_bwrd(m, md);
}
