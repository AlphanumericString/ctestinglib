/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_display_tests.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:31:54 by bgoulard          #+#    #+#             */
/*   Updated: 2026/01/22 13:00:27 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

#define CTESTS_INTERNAL
#include "tc_structs.h"
#include "tc_modules.h"
#include "tc_utils.h"

// if (WCOREDUMP(t->return_value))	not iso c99 :(
// 	print_string(" (core dumped)");
// print_string("\t");
// print_string(strsignal(WTERMSIG(t->return_value)));

// static void	loc_print_signal_info(int return_value) __attribute__((unused));
// static void	loc_print_signal_info(int return_value)
// {
// 	if (WIFSIGNALED(return_value))
// 	{
// 		tci_print_string(" (signal: ");
// 		tci_print_nb(WTERMSIG(return_value));
// 		tci_print_string(")");
// 	}
// }

static void	loc_td_res(bool test_success, t_display_mode md, int return_value)
{
	if ((md & TC_FMT_MSK) == TC_FMT_TXT)
	{
		if (test_success == true)
			return ((void)tci_print_string(OK_COLOR "OK" RESET_COLOR "\n"));
		tci_print_string(KO_COLOR "KO" RESET_COLOR "\t");
		tci_print_nb(return_value);
		tci_print_string("\n");
		return ;
	}
	else if ((md & TC_FMT_MSK) == TC_FMT_JSN)
	{
		tci_print_string("\"result\":\"");
		if (test_success == true)
			tci_print_string("OK\"");
		else
			tci_print_string("KO\"");
		tci_print_string(",\"return_value\":\"");
		tci_print_nb(return_value);
		tci_print_string("\",\"signaled\":\"");
		if (WIFSIGNALED(return_value))
			tci_print_string("true\"");
		else
			tci_print_string("false\"");
		return (tci_print_string("}"));
	}
}

static void	test_res_disp(const t_test *t, const t_module *m, t_display_mode md,
			size_t	block[2])
{
	bool		was_test_successfull;
	const int	e_s = EXIT_SUCCESS;

	was_test_successfull = false;
	if (WIFEXITED(t->return_value) && WEXITSTATUS(t->return_value) == e_s)
		was_test_successfull = true;
	if ((md & TC_DT_RESMSK) != TC_DT_OK && was_test_successfull == true)
		return ;
	tci_td_name(t, m, md, block);
	loc_td_res(was_test_successfull, md, t->return_value);
}

static void	loc_td_open_block(t_display_mode md)
{
	if ((md & TC_FMT_MSK) == TC_FMT_TXT)
		return ;
	if ((md & TC_FMT_MSK) == TC_FMT_JSN)
		tci_print_string("\"tests\":[");
}

static void	loc_td_close_block(t_display_mode md)
{
	if ((md & TC_FMT_MSK) == TC_FMT_TXT)
		return ;
	if ((md & TC_FMT_MSK) == TC_FMT_JSN)
		tci_print_string("]");
}

void	tci_mdisplay_tests(const t_module *m, t_display_mode md, int depth)
{
	t_list	*t_ls;
	size_t	mlen;

	t_ls = m->tests_list;
	mlen = tci_mget_longest_name(m) + 2;
	loc_td_open_block(md);
	while (t_ls)
	{
		test_res_disp(t_ls->data, m, md, (size_t[2]){(size_t)depth, mlen});
		if ((md & TC_FMT_MSK) == TC_FMT_JSN && t_ls->next)
			tci_print_string(",");
		t_ls = t_ls->next;
	}
	loc_td_close_block(md);
}
