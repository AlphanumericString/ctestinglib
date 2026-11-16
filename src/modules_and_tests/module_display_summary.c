#define CTESTS_INTERNAL
#include "ct_internal.h"

void cti_mdisplay_module_summary(const t_module *m, t_display_mode md)
{
	int test_total;
	int test_passed;

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
	if ((md & CT_DM_SUMMSK) == CT_DM_SUMNBS)
		cti_mdisplay_sum_nb(test_total, test_passed, md);
	else if ((md & CT_DM_SUMMSK) == CT_DM_SUMPCT)
		cti_mdisplay_sum_percent(test_total, test_passed, md);
}
