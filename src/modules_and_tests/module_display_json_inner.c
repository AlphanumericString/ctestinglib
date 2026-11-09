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
}

static void	loc_md_json_fwrd(const t_module *m, t_display_mode md)
{
	(void)md;
	cti_print_string("{\"module_name\":\"");
	cti_print_string(m->name);
	cti_print_string("\",\"description\":\"");
	cti_print_string(m->description);
	cti_print_string("\",");
}

static void	loc_md_json_bwrd(const t_module *m, t_display_mode md)
{
	module_display_tests_infos(m, md);
	cti_print_string("}");
}

void	cti_display_results_json(const t_module *m, t_display_mode md)
{
	t_list		*md_l;

	md_l = m->submodules_list;
	loc_md_json_fwrd(m, md);
	cti_mdisplay_tests(m, md, 0);
	cti_print_string(",");
	cti_print_string("\"submodules\":[");
	while (md_l)
	{
		cti_display_results_json(md_l->data, md);
		if (md_l->next)
			cti_print_string(",");
		md_l = md_l->next;
	}
	cti_print_string("]");
	if ((md & CT_DM_SUMMSK) != CT_DM_SUMSILENT)
		cti_print_string(",");
	loc_md_json_bwrd(m, md);
}
