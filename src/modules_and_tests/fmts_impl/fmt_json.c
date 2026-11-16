#include <sys/wait.h>

#define CTESTS_INTERNAL
#include "ct_internal.h"

static void loc_fmt_json_open(const t_module *m, t_display_mode md, int depth)
{
	cti_print_string("{\"module_name\":\"");
	cti_print_string(m->name);
	cti_print_string("\",\"description\":\"");
	cti_print_string(m->description);
	cti_print_string("\",");
}

static void loc_td_res_json(const t_test *t, bool test_success)
{
	cti_print_string("\"result\":\"");
	if (test_success == true)
		cti_print_string("OK\"");
	else
		cti_print_string("KO\"");
	cti_print_string(",\"return_value\":\"");
	cti_print_nb(t->return_value);
	cti_print_string("\",\"timed_out\":\"");
	if (t->timed_out == true)
		cti_print_string("true\"");
	else
		cti_print_string("false\"");
	cti_print_string(",\"signaled\":\"");
	if (WIFSIGNALED(t->return_value))
		cti_print_string("true\"");
	else
		cti_print_string("false\"");
	cti_print_string("}");
}

static void loc_td_test_json(const t_test *t, const t_module *m,
							 t_display_mode md)
{
	cti_print_string("{\"test_name\":\"");
	cti_mdisplay_test_name(t, m, md);
	cti_print_string("\",");
	loc_td_res_json(t, cti_test_succeeded(t));
}

static void loc_fmt_json_tests(const t_module *m, t_display_mode md, int depth)
{
	const t_list *tests;

	cti_print_string("\"tests\":[");
	tests = m->tests_list;
	while (tests)
	{
		loc_td_test_json(tests->data, m, md);
		if (tests->next)
			cti_print_string(",");
		tests = tests->next;
	}
	cti_print_string("],");
}

static void loc_fmt_json_modules(const t_module *m, t_display_mode md,
								 int depth)
{
	const t_list *sbm;

	sbm = m->submodules_list;
	cti_print_string("\"submodules\":[");
	while (sbm)
	{
		cti_display_module(sbm->data, md, depth + 1);
		if (sbm->next)
			cti_print_string(",");
		sbm = sbm->next;
	}
	cti_print_string("]");
}

static void loc_fmt_json_summary(const t_module *m, t_display_mode md,
								 int depth)
{
	if ((md & CT_DM_SUMMSK) == CT_DM_SUMSILENT)
		return;
	cti_print_string(",");
	cti_mdisplay_module_summary(m, md);
}

static void loc_fmt_json_close(const t_module *m, t_display_mode md, int depth)
{
	cti_print_string("}");
}

static void loc_fmt_json_last_close(const t_module *m, t_display_mode md,
									int depth)
{
	cti_print_string("\n");
}

static void loc_percent_json(int t_t, int t_p, t_display_mode md)
{
	if (!t_t)
		return ((void)cti_print_string("\"percentage\":\"NaN%\""));
	cti_print_string("\"percentage\":\"");
	cti_print_flt((double)t_p / t_t * 100);
	cti_print_string("%\"");
}

static void loc_nb_json(int t_t, int t_p, t_display_mode md)
{
	cti_print_string("\"nb_tests_passed\":\"");
	cti_print_nb(t_p);
	cti_print_string("\",\"nb_tests_total\":\"");
	cti_print_nb(t_t);
	cti_print_string("\"");
}

const t_display_fmt g_cti_fmt_json = {
	.open = loc_fmt_json_open,
	.tests = loc_fmt_json_tests,
	.modules = loc_fmt_json_modules,
	.summary = loc_fmt_json_summary,
	.close = loc_fmt_json_close,
	.last_close = loc_fmt_json_last_close,
	.sum_nb = loc_nb_json,
	.sum_percent = loc_percent_json,
};
