#include <stdio.h>
#define CTESTS_INTERNAL
#include "ct_modules.h"
#include "ct_utils.h"

static void	module_display_tests_infos(const t_module *m, t_display_mode md)
{
	int	test_total;
	int	test_passed;

	test_total = cti_collect_sbm_t_total(m);
	test_passed = cti_collect_sbm_t_success(m);
	if ((md & CT_DSBM_GRPMSK) == CT_DSBM_SEP)
	{
		test_total = m->tests_count;
		test_passed = m->tests_passed;
	}
	if ((md & CT_DM_SUMMSK) == CT_DM_SUMNBS)
		cti_mdisplay_sum_nb(test_total, test_passed, md);
	if ((md & CT_DM_SUMMSK) == CT_DM_SUMPCT)
		cti_mdisplay_sum_percent(test_total, test_passed, md);
	if ((md & CT_DM_SUMMSK) != CT_DM_SUMSILENT && m->submodules_list)
		cti_print_string("\t");
	cti_mdisplay_sbm(m->submodules_list, md);
}

static void	module_print(const t_module *m, int depth, bool fwrd)
{
	cti_print_charn(depth, '\t');
	cti_print_string(m->name);
	if (!fwrd)
		return ;
	cti_print_string("\t");
	cti_print_string(m->description);
}

static void	cti_mdisplay_fwrd(const t_module *m, t_display_mode md, int depth)
{
	if ((md & CT_DM_SUMMSK) == CT_DM_SUMSILENT)
		return ;
	module_print(m, depth, true);
	cti_print_string("\n");
}

static void	cti_mdisplay_bwrd(const t_module *m, t_display_mode md, int depth)
{
	if ((md & CT_DM_SUMMSK) == CT_DM_SUMSILENT)
		return ;
	if ((md & CT_DM_SUMMSK) == CT_DM_SUMNBS
		&& !((md & CT_DSBM_GRPMSK) == CT_DSBM_MRG)
		&& m->tests_count == 0)
		return ;
	module_print(m, depth, false);
	if ((md & CT_DM_SUMMSK) == CT_DM_SUMSILENT
		|| ((md & CT_DSBM_GRPMSK) == CT_DSBM_SEP && m->submodules_list == NULL))
		return ((void)cti_print_string("\n"));
	cti_print_string("\t");
	module_display_tests_infos(m, md);
}

void	cti_display_results_txt(const t_module *m, t_display_mode md, int depth)
{
	const int	dp = depth;
	t_list		*md_l;

	md_l = m->submodules_list;
	cti_mdisplay_fwrd(m, md, depth);
	while (md_l)
	{
		cti_display_results_txt(md_l->data, md, dp + 1);
		md_l = md_l->next;
	}
	cti_mdisplay_tests(m, md, depth);
	cti_mdisplay_bwrd(m, md, depth);
}
