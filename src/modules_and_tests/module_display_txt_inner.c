#define CTESTS_INTERNAL
#include "ct_internal.h"

static void loc_md_txt_name(const t_module *m, int depth, bool description)
{
	cti_print_charn(depth, '\t');
	cti_print_string(m->name);
	if (!description)
		return;
	cti_print_string("\t");
	cti_print_string(m->description);
}

static void loc_md_txt_open(const t_module *m, t_display_mode md, int depth)
{
	if ((md & CT_DM_SUMMSK) == CT_DM_SUMSILENT)
		return;
	loc_md_txt_name(m, depth, true);
	cti_print_string("\n");
}

static bool loc_md_txt_close_skipped(const t_module *m, t_display_mode md)
{
	if ((md & CT_DM_SUMMSK) == CT_DM_SUMSILENT)
		return (true);
	if ((md & CT_DM_SUMMSK) == CT_DM_SUMNBS &&
		(md & CT_DSBM_GRPMSK) != CT_DSBM_MRG && m->tests_count == 0)
		return (true);
	return (false);
}

static void loc_md_txt_summary(const t_module *m, t_display_mode md)
{
	cti_mdisplay_module_summary(m, md);
	if (m->submodules_list)
		cti_print_string("\t");
	cti_mdisplay_sbm_recap(m->submodules_list, md);
}

static void loc_md_txt_close(const t_module *m, t_display_mode md, int depth)
{
	if (loc_md_txt_close_skipped(m, md))
		return;
	loc_md_txt_name(m, depth, false);
	if ((md & CT_DSBM_GRPMSK) == CT_DSBM_SEP && m->submodules_list == NULL)
		return ((void)cti_print_string("\n"));
	cti_print_string("\t");
	loc_md_txt_summary(m, md);
}

static void loc_md_txt_submodules(const t_module *m, t_display_mode md,
								  int depth)
{
	const t_list *sbm;

	sbm = m->submodules_list;
	while (sbm)
	{
		cti_display_results_txt(sbm->data, md, depth + 1);
		sbm = sbm->next;
	}
}

void cti_display_results_txt(const t_module *m, t_display_mode md, int depth)
{
	loc_md_txt_open(m, md, depth);
	loc_md_txt_submodules(m, md, depth);
	cti_mdisplay_tests_txt(m, md, depth);
	loc_md_txt_close(m, md, depth);
}
