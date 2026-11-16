#include <string.h>

#define CTESTS_INTERNAL
#include "ct_internal.h"

#define KO_COLOR "\033[31m"
#define OK_COLOR "\033[32m"
#define RESET_COLOR "\033[0m"

static void loc_md_txt_name(const t_module *m, int depth, bool description)
{
	cti_print_charn(depth, '\t');
	cti_print_string(m->name);
	if (!description)
		return;
	cti_print_string("\t");
	cti_print_string(m->description);
}

static void loc_fmt_txt_open(const t_module *m, t_display_mode md, int depth)
{
	if ((md & CT_DM_SUMMSK) == CT_DM_SUMSILENT)
		return;
	loc_md_txt_name(m, depth, true);
	cti_print_string("\n");
}

static void loc_td_res_txt(const t_test *t, bool test_success)
{
	if (test_success == true)
		return ((void)cti_print_string(OK_COLOR "OK" RESET_COLOR "\n"));
	cti_print_string(KO_COLOR "KO" RESET_COLOR "\t");
	if (t->timed_out == true)
		cti_print_string("TIMEOUT\n");
	else
	{
		cti_print_nb(t->return_value);
		cti_print_string("\n");
	}
}

static void loc_td_test_txt(const t_test *t, const t_module *m,
							t_display_mode md, int depth, size_t mlen)
{
	bool test_success;

	test_success = cti_test_succeeded(t);
	if ((md & CT_DT_RESMSK) != CT_DT_OK && test_success == true)
		return;
	if ((md & CT_DM_SUMMSK) != CT_DM_SUMSILENT)
		cti_print_charn(depth + 1, '\t');
	cti_mdisplay_test_name(t, m, md);
	cti_print_charn(mlen - strlen(t->name), ' ');
	loc_td_res_txt(t, test_success);
}

static void loc_fmt_txt_tests(const t_module *m, t_display_mode md, int depth)
{
	const t_list *tests;
	size_t		  mlen;

	tests = m->tests_list;
	mlen = cti_mget_longest_name(m) + 2;
	while (tests)
	{
		loc_td_test_txt(tests->data, m, md, depth, mlen);
		tests = tests->next;
	}
}

static void loc_fmt_txt_modules(const t_module *m, t_display_mode md, int depth)
{
	const t_list *sbm;

	sbm = m->submodules_list;
	while (sbm)
	{
		cti_display_module(sbm->data, md, depth + 1);
		sbm = sbm->next;
	}
}

static bool loc_fmt_txt_close_skipped(const t_module *m, t_display_mode md)
{
	if ((md & CT_DM_SUMMSK) == CT_DM_SUMSILENT)
		return (true);
	if ((md & CT_DM_SUMMSK) == CT_DM_SUMNBS &&
		(md & CT_DSBM_GRPMSK) != CT_DSBM_MRG && m->tests_count == 0)
		return (true);
	return (false);
}

static void loc_fmt_txt_summary(const t_module *m, t_display_mode md, int depth)
{
	if (loc_fmt_txt_close_skipped(m, md))
		return;
	loc_md_txt_name(m, depth, false);
	if ((md & CT_DSBM_GRPMSK) == CT_DSBM_SEP && m->submodules_list == NULL)
		return;
	cti_print_string("\t");
	cti_mdisplay_module_summary(m, md);
	if (m->submodules_list)
		cti_print_string("\t");
	cti_mdisplay_sbm_recap(m->submodules_list, md);
}

static void loc_fmt_txt_close(const t_module *m, t_display_mode md, int depth)
{
	if (loc_fmt_txt_close_skipped(m, md))
		return;
	cti_print_string("\n");
}

static void loc_percent_txt(int t_t, int t_p, t_display_mode md)
{
	const char *str_color[] = {KO_COLOR, OK_COLOR};

	if (!t_t)
		return ((void)cti_print_string("NaN%"));
	cti_print_string("(");
	if ((md & CT_PRTY_MSK) == CT_PRTY_COLOR)
		cti_print_string(str_color[t_p == t_t]);
	cti_print_flt((double)t_p / t_t * 100);
	if ((md & CT_PRTY_MSK) == CT_PRTY_COLOR)
		cti_print_string("\033[0m"
						 "%)");
}

static void loc_nb_txt(int t_t, int t_p, t_display_mode md)
{
	const char *str_color[] = {KO_COLOR, OK_COLOR};

	if (!t_t)
		return ((void)cti_print_string("empty"));
	if ((md & CT_PRTY_MSK) == CT_PRTY_COLOR)
		cti_print_string(str_color[t_p == t_t]);
	cti_print_nb(t_p);
	if ((md & CT_PRTY_MSK) == CT_PRTY_COLOR)
		cti_print_string("\033[0m");
	cti_print_string(" / ");
	cti_print_nb(t_t);
}

static void loc_sbm_recap_txt(const t_list *sbm, t_display_mode md)
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
}

static void loc_display_results_txt_list(t_module **selection, size_t nb,
										 t_display_mode mode)
{
	for (size_t i = 0; i < nb; i++)
		cti_display_module(selection[i], mode, 0);
}

const t_display_fmt g_cti_fmt_txt = {
	.open = loc_fmt_txt_open,
	.tests = loc_fmt_txt_tests,
	.modules = loc_fmt_txt_modules,
	.summary = loc_fmt_txt_summary,
	.close = loc_fmt_txt_close,
	.sum_nb = loc_nb_txt,
	.sum_percent = loc_percent_txt,
	.sbm_recap = loc_sbm_recap_txt,
	.md_list_displayer = loc_display_results_txt_list,
	.submodules_first = true,
};
