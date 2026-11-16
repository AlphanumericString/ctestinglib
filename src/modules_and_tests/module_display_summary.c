#define CTESTS_INTERNAL
#include "ct_internal.h"

void cti_mdisplay_module_summary(const t_module *m, t_display_mode md)
{
	const t_display_fmt *fmt;
	int					 test_total;
	int					 test_passed;

	if ((md & CT_DSBM_GRPMSK) == CT_DSBM_SEP)
	{
		test_total = m->tests_count;
		test_passed = m->tests_passed;
	}
	else
	{
		test_total = cti_collect_sbm_t_total(m);
		test_passed = cti_collect_sbm_t_success(m);
	}
	fmt = cti_display_fmt_get(md);
	if (!fmt)
		return;
	if ((md & CT_DM_SUMMSK) == CT_DM_SUMNBS && fmt->sum_nb)
		fmt->sum_nb(test_total, test_passed, md);
	else if ((md & CT_DM_SUMMSK) == CT_DM_SUMPCT && fmt->sum_percent)
		fmt->sum_percent(test_total, test_passed, md);
}

void cti_mdisplay_sum_nb(int t_t, int t_p, t_display_mode md)
{
	const t_display_fmt *fmt;

	fmt = cti_display_fmt_get(md);
	if (!fmt || !fmt->sum_nb)
		return ((void)cti_print_string(
			"Unsupported format for percentage display\n"));
	fmt->sum_nb(t_t, t_p, md);
}

void cti_mdisplay_sum_percent(int t_t, int t_p, t_display_mode md)
{
	const t_display_fmt *fmt;

	fmt = cti_display_fmt_get(md);
	if (!fmt || !fmt->sum_percent)
		return ((void)cti_print_string(
			"Unsupported format for percentage display\n"));
	fmt->sum_percent(t_t, t_p, md);
}

void cti_mdisplay_sbm_recap(const t_list *sbm, t_display_mode md)
{
	const t_display_fmt *fmt;

	fmt = cti_display_fmt_get(md);
	if (!fmt || !fmt->sbm_recap)
		return ((void)cti_print_string(
			"Unsupported format for submodule display\n"));
	fmt->sbm_recap(sbm, md);
}
