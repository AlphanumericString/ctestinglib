#define CTESTS_INTERNAL
#include "ct_modules.h"
#include "ct_utils.h"

static void	loc_display_sbm_txt(const t_list *sbm, t_display_mode md)
{
	t_module	*m;
	int			t_t;

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

// json doesn't have one as they "print in place" and dont do a recap
// for each sub module but simple nests them in their parents
void	cti_mdisplay_sbm(const t_list *sbm, t_display_mode md)
{
	if ((md & CT_FMT_MSK) == CT_FMT_TXT)
		return (loc_display_sbm_txt(sbm, md));
	cti_print_string("Unsupported format for submodule display\n");
}
