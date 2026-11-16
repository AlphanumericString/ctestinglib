#define CTESTS_INTERNAL
#include "ct_internal.h"

static void cti_mdisplay_chain(const t_module *m)
{
	if (m->parent)
		cti_mdisplay_chain(m->parent);
	cti_print_string(m->name);
	cti_print_string(":");
}

static void loc_td_name(const t_test *t, const t_module *m, t_display_mode md)
{
	if ((md & CT_DT_NAMEMSK) == CT_DT_FULLNAME)
		cti_mdisplay_chain(m);
	cti_print_string(t->name);
}

void cti_td_name_txt(const t_test *t, const t_module *m, t_display_mode md,
					 int depth, size_t mlen)
{
	if ((md & CT_DM_SUMMSK) != CT_DM_SUMSILENT)
		cti_print_charn(depth + 1, '\t');
	loc_td_name(t, m, md);
	cti_print_charn(mlen - ct_strlen(t->name), ' ');
}

void cti_td_name_json(const t_test *t, const t_module *m, t_display_mode md)
{
	cti_print_string("{\"test_name\":\"");
	loc_td_name(t, m, md);
	cti_print_string("\",");
}
