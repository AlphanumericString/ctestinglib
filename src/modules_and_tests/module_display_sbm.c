#define CTESTS_INTERNAL
#include "ct_internal.h"

static void loc_display_sbm_txt(const t_list *sbm, t_display_mode md)
{
	const t_module *m;
	int				t_t;

	while ((md & CT_DM_SUMMSK) != CT_DM_SUMSILENT && sbm)
	{
		m = sbm->data;
		t_t = cti_collect_sbm_t_total(m);
		sbm = sbm->next;
		cti_print_string("[");
		cti_print_string(m->name);
		cti_print_string(": ");
		if ((md & CT_DSBM_SUMMSK) == CT_DSBM_SUMPCT)
			cti_mdisplay_sum_percent(t_t, cti_collect_sbm_t_success(m), md);
		else
			cti_mdisplay_sum_nb(t_t, cti_collect_sbm_t_success(m), md);
		cti_print_string("]");
	}
	cti_print_string("\n");
}

// json doesn't have one as they print every fields in a recursive maner.
// hence the SUMMARY
void cti_mdisplay_sbm_recap(const t_list *sbm, t_display_mode md)
{
	switch (md & CT_FMT_MSK)
	{
		case CT_FMT_TXT:
			return (loc_display_sbm_txt(sbm, md));
		default:
			return ((void)cti_print_string(
				"Unsupported format for submodule display\n"));
	}
}
