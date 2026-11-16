#include <stdlib.h>
#include <sys/wait.h>

#define CTESTS_INTERNAL
#include "ct_internal.h"

static bool test_succeeded(const t_test *t)
{
	const int success_code = EXIT_SUCCESS;

	return (WIFEXITED(t->return_value) &&
			WEXITSTATUS(t->return_value) == success_code);
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

static void loc_td_test_txt(const t_test *t, const t_module *m,
							t_display_mode md, int depth, size_t mlen)
{
	bool test_success;

	test_success = test_succeeded(t);
	if ((md & CT_DT_RESMSK) != CT_DT_OK && test_success == true)
		return;
	cti_td_name_txt(t, m, md, depth, mlen);
	loc_td_res_txt(t, test_success);
}

static void loc_td_test_json(const t_test *t, const t_module *m,
							 t_display_mode md)
{
	cti_td_name_json(t, m, md);
	loc_td_res_json(t, test_succeeded(t));
}

void cti_mdisplay_tests_txt(const t_module *m, t_display_mode md, int depth)
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

void cti_mdisplay_tests_json(const t_module *m, t_display_mode md)
{
	const t_list *tests;

	tests = m->tests_list;
	while (tests)
	{
		loc_td_test_json(tests->data, m, md);
		if (tests->next)
			cti_print_string(",");
		tests = tests->next;
	}
}
