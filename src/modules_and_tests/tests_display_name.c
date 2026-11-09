#define CTESTS_INTERNAL
#include "ct_modules.h"
#include "ct_utils.h"

static void	cti_mdisplay_chain(const t_module *m)
{
	if (m->parent)
		cti_mdisplay_chain(m->parent);
	cti_print_string(m->name);
	cti_print_string(":");
}

static void	loc_hdl_fmts(int depth, t_display_mode md)
{
	if ((md & CT_DM_SUMMSK) != CT_DM_SUMSILENT
		&& (md & CT_FMT_MSK) == CT_FMT_TXT)
		return (cti_print_charn(depth + 1, '\t'));
	if ((md & CT_FMT_MSK) == CT_FMT_JSN)
		return (cti_print_string("{\"test_name\":\""));
}

static void	loc_cls_fmts(const t_test *t, size_t ml, t_display_mode md)
{
	if ((md & CT_FMT_MSK) == CT_FMT_JSN)
		return (cti_print_string("\","), (void)0);
	else if ((md & CT_FMT_MSK) == CT_FMT_TXT)
		cti_print_charn(ml - ct_strlen(t->name), ' ');
}

void	cti_td_name(const t_test *t, const t_module *m, t_display_mode md,
			size_t *pass)
{
	const size_t	mlen = pass[1];
	const int		depth = (int)pass[0];

	loc_hdl_fmts(depth, md);
	if ((md & CT_DT_NAMEMSK) == CT_DT_FULLNAME)
		cti_mdisplay_chain(m);
	cti_print_string(t->name);
	loc_cls_fmts(t, mlen, md);
}
