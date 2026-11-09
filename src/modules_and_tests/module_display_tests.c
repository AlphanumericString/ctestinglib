#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

#define CTESTS_INTERNAL
#include "ct_structs.h"
#include "ct_modules.h"
#include "ct_utils.h"

// if (WCOREDUMP(t->return_value))	not iso c99 :(
// 	print_string(" (core dumped)");
// print_string("\t");
// print_string(strsignal(WTERMSIG(t->return_value)));

// static void	loc_print_signal_info(int return_value) __attribute__((unused));
// static void	loc_print_signal_info(int return_value)
// {
// 	if (WIFSIGNALED(return_value))
// 	{
// 		cti_print_string(" (signal: ");
// 		cti_print_nb(WTERMSIG(return_value));
// 		cti_print_string(")");
// 	}
// }

static void loc_td_res(const t_test *t, bool test_success, t_display_mode md)
{
	if ((md & CT_FMT_MSK) == CT_FMT_TXT)
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
		return;
	}
	else if ((md & CT_FMT_MSK) == CT_FMT_JSN)
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
		return (cti_print_string("}"));
	}
}

static void test_res_disp(const t_test *t, const t_module *m, t_display_mode md,
						  size_t block[2])
{
	bool	  was_test_successfull;
	const int e_s = EXIT_SUCCESS;

	was_test_successfull = false;
	if (WIFEXITED(t->return_value) && WEXITSTATUS(t->return_value) == e_s)
		was_test_successfull = true;
	if ((md & CT_DT_RESMSK) != CT_DT_OK && was_test_successfull == true &&
		(md & CT_FMT_MSK) != CT_FMT_JSN)
		return;
	cti_td_name(t, m, md, block);
	loc_td_res(t, was_test_successfull, md);
}

static void loc_td_open_block(t_display_mode md)
{
	if ((md & CT_FMT_MSK) == CT_FMT_TXT)
		return;
	if ((md & CT_FMT_MSK) == CT_FMT_JSN)
		cti_print_string("\"tests\":[");
}

static void loc_td_close_block(t_display_mode md)
{
	if ((md & CT_FMT_MSK) == CT_FMT_TXT)
		return;
	if ((md & CT_FMT_MSK) == CT_FMT_JSN)
		cti_print_string("]");
}

void cti_mdisplay_tests(const t_module *m, t_display_mode md, int depth)
{
	t_list *t_ls;
	size_t	mlen;

	t_ls = m->tests_list;
	mlen = cti_mget_longest_name(m) + 2;
	loc_td_open_block(md);
	while (t_ls)
	{
		test_res_disp(t_ls->data, m, md, (size_t[2]){(size_t)depth, mlen});
		if ((md & CT_FMT_MSK) == CT_FMT_JSN && t_ls->next)
			cti_print_string(",");
		t_ls = t_ls->next;
	}
	loc_td_close_block(md);
}
