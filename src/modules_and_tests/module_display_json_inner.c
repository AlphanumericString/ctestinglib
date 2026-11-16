#define CTESTS_INTERNAL
#include "ct_internal.h"

static void loc_md_json_open(const t_module *m)
{
	cti_print_string("{\"module_name\":\"");
	cti_print_string(m->name);
	cti_print_string("\",\"description\":\"");
	cti_print_string(m->description);
	cti_print_string("\",");
}

static void loc_md_json_tests(const t_module *m, t_display_mode md)
{
	cti_print_string("\"tests\":[");
	cti_mdisplay_tests_json(m, md);
	cti_print_string("],");
}

static void loc_md_json_submodules(const t_module *m, t_display_mode md)
{
	const t_list *sbm;

	sbm = m->submodules_list;
	cti_print_string("\"submodules\":[");
	while (sbm)
	{
		cti_display_results_json(sbm->data, md);
		if (sbm->next)
			cti_print_string(",");
		sbm = sbm->next;
	}
	cti_print_string("]");
}

static void loc_md_json_summary(const t_module *m, t_display_mode md)
{
	if ((md & CT_DM_SUMMSK) == CT_DM_SUMSILENT)
		return;
	cti_print_string(",");
	cti_mdisplay_module_summary(m, md);
}

void cti_display_results_json(const t_module *m, t_display_mode md)
{
	loc_md_json_open(m);
	loc_md_json_tests(m, md);
	loc_md_json_submodules(m, md);
	loc_md_json_summary(m, md);
	cti_print_string("}");
}
