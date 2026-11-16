#include <sys/wait.h>

#define CTESTS_INTERNAL
#include "ct_internal.h"

// writes str with the characters that are significant in html escaped
static void loc_html_escape(const char *str)
{
	char c;

	if (!str)
		return;
	while (*str)
	{
		c = *str++;
		if (c == '&')
			cti_print_string("&amp;");
		else if (c == '<')
			cti_print_string("&lt;");
		else if (c == '>')
			cti_print_string("&gt;");
		else if (c == '"')
			cti_print_string("&quot;");
		else if (c == '\'')
			cti_print_string("&#39;");
		else
			cti_print_charn(1, c);
	}
}

static void loc_html_doc_open(void)
{
	cti_print_string("<!DOCTYPE html>\n"
					 "<html lang=\"en\">\n"
					 "<head>\n"
					 "<meta charset=\"utf-8\">\n"
					 "<title>ctests report</title>\n"
					 "</head>\n"
					 "<body>\n"
					 "<main class=\"ctests\">\n");
}

static void loc_html_doc_close(void)
{
	cti_print_string("</main>\n"
					 "</body>\n"
					 "</html>\n");
}

static void loc_fmt_html_first_open(const t_module *m, t_display_mode md,
									int depth)
{
	loc_html_doc_open();
}

static void loc_fmt_html_open(const t_module *m, t_display_mode md, int depth)
{
	cti_print_string("<section class=\"module\" data-depth=\"");
	cti_print_nb(depth);
	cti_print_string("\">\n"
					 "<header class=\"module-header\">\n"
					 "<h2>");
	loc_html_escape(m->name);
	cti_print_string("</h2>\n"
					 "<p class=\"module-description\">");
	loc_html_escape(m->description);
	cti_print_string("</p>\n"
					 "</header>\n");
}

static void loc_html_test(const t_test *t, const t_module *m, t_display_mode md)
{
	bool success;

	success = cti_test_succeeded(t);
	cti_print_string("<li class=\"test ");
	cti_print_string(success ? "ok" : "ko");
	cti_print_string("\">\n"
					 "<span class=\"test-name\">");
	cti_mdisplay_test_name(t, m, md);
	cti_print_string("</span>\n"
					 "<span class=\"test-result\">");
	cti_print_string(success ? "OK" : "KO");
	cti_print_string("</span>\n"
					 "<span class=\"test-return-value\">");
	cti_print_nb(t->return_value);
	cti_print_string("</span>\n"
					 "<span class=\"test-timed-out\">");
	cti_print_string(t->timed_out ? "true" : "false");
	cti_print_string("</span>\n"
					 "<span class=\"test-signaled\">");
	cti_print_string(WIFSIGNALED(t->return_value) ? "true" : "false");
	cti_print_string("</span>\n"
					 "</li>\n");
}

static void loc_fmt_html_tests(const t_module *m, t_display_mode md, int depth)
{
	const t_list *tests;

	cti_print_string("<ul class=\"tests\">\n");
	tests = m->tests_list;
	while (tests)
	{
		loc_html_test(tests->data, m, md);
		tests = tests->next;
	}
	cti_print_string("</ul>\n");
}

static void loc_fmt_html_modules(const t_module *m, t_display_mode md,
								 int depth)
{
	const t_list *sbm;

	cti_print_string("<div class=\"submodules\">\n");
	sbm = m->submodules_list;
	while (sbm)
	{
		cti_display_module(sbm->data, md, depth + 1);
		sbm = sbm->next;
	}
	cti_print_string("</div>\n");
}

static void loc_fmt_html_summary(const t_module *m, t_display_mode md,
								 int depth)
{
	if ((md & CT_DM_SUMMSK) == CT_DM_SUMSILENT)
		return;
	cti_print_string("<footer class=\"summary\">\n");
	cti_mdisplay_module_summary(m, md);
	cti_print_string("\n");
	if (m->submodules_list)
	{
		cti_mdisplay_sbm_recap(m->submodules_list, md);
		cti_print_string("\n");
	}
	cti_print_string("</footer>\n");
}

static void loc_fmt_html_close(const t_module *m, t_display_mode md, int depth)
{
	cti_print_string("</section>\n");
}

static void loc_fmt_html_last_close(const t_module *m, t_display_mode md,
									int depth)
{
	loc_html_doc_close();
}

static void loc_percent_html(int t_t, int t_p, t_display_mode md)
{
	cti_print_string("<span class=\"summary-percent\">");
	if (!t_t)
		cti_print_string("NaN%");
	else
	{
		cti_print_flt((double)t_p / t_t * 100);
		cti_print_string("%");
	}
	cti_print_string("</span>");
}

static void loc_nb_html(int t_t, int t_p, t_display_mode md)
{
	cti_print_string("<span class=\"summary-nb\">");
	cti_print_nb(t_p);
	cti_print_string(" / ");
	cti_print_nb(t_t);
	cti_print_string("</span>");
}

static void loc_sbm_recap_html(const t_list *sbm, t_display_mode md)
{
	const t_module *m;
	int				t_t;

	while ((md & CT_DM_SUMMSK) != CT_DM_SUMSILENT && sbm)
	{
		m = sbm->data;
		t_t = cti_collect_sbm_t_total(m);
		sbm = sbm->next;
		cti_print_string("<span class=\"submodule\">"
						 "<span class=\"submodule-name\">");
		loc_html_escape(m->name);
		cti_print_string("</span>");
		if ((md & CT_DSBM_SUMMSK) == CT_DSBM_SUMPCT)
			cti_mdisplay_sum_percent(t_t, cti_collect_sbm_t_success(m), md);
		else
			cti_mdisplay_sum_nb(t_t, cti_collect_sbm_t_success(m), md);
		cti_print_string("</span>");
	}
}

// wraps a module selection in one standalone html document
static void loc_display_results_html_list(t_module **modules, size_t count,
										  t_display_mode md)
{
	size_t i;

	loc_html_doc_open();
	i = 0;
	while (i < count)
	{
		cti_display_module(modules[i], md, 0);
		i++;
	}
	loc_html_doc_close();
}

const t_display_fmt g_cti_fmt_html = {
	.first_open = loc_fmt_html_first_open,
	.open = loc_fmt_html_open,
	.tests = loc_fmt_html_tests,
	.modules = loc_fmt_html_modules,
	.summary = loc_fmt_html_summary,
	.close = loc_fmt_html_close,
	.last_close = loc_fmt_html_last_close,
	.sum_nb = loc_nb_html,
	.sum_percent = loc_percent_html,
	.sbm_recap = loc_sbm_recap_html,
	.md_list_displayer = loc_display_results_html_list,
	.submodules_first = false,
};
