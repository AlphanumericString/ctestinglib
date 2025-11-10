/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_display.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:31:54 by bgoulard          #+#    #+#             */
/*   Updated: 2025/08/08 18:32:34 by bgoulard         ###   ########.fr       */
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

static void	test_name_display(const t_test *t, const t_module *m, t_display_mode md,
				size_t *pass)
{
	size_t			len;
	const size_t	mlen = pass[1];
	const int		depth = (int)pass[0];

	if ((md & TC_DM_SUMMARY_MASK) == TC_DM_SILENT)
		tci_print_charn(depth + 1, '\t');
	if ((md & TC_DT_NAMMASK) == TC_DT_FULLNAME)
			tci_mdisplay_chain(m);
	tci_print_string(t->name);
	len = 0;
	while (t->name[len])
		len++;
	tci_print_charn(mlen - len, ' ');
}

static void	test_res_disp(const t_test *t, const t_module *m, t_display_mode md,
			size_t	block[2])
{
	bool			was_test_successfull;

	was_test_successfull = false;
	if (WIFEXITED(t->return_value) && WEXITSTATUS(t->return_value) == EXIT_SUCCESS)
		was_test_successfull = true;
	if ((md & TC_DT_RESMASK) != TC_DT_OK && was_test_successfull == true)
		return ;
	test_name_display(t, m, md, block);
	if (was_test_successfull == true && (md & TC_DT_RESMASK) == TC_DT_OK)
		return ((void)tci_print_string(OK_COLOR "OK" RESET_COLOR "\n"));
	tci_print_string(KO_COLOR "KO" RESET_COLOR "\t");
	if (!WIFEXITED(t->return_value))
	{
		tci_print_string("Signal: ");
		tci_print_nb(WTERMSIG(t->return_value));
	}
	else
		tci_print_nb(WEXITSTATUS(t->return_value));
	tci_print_string("\n");
}


void	tci_mdisplay_tests(const t_module *m, t_display_mode md, int depth)
{
	t_list	*t_ls;
	size_t	mlen;

	t_ls = m->tests_list;
	mlen = tci_mget_longest_name(m) + 2;
	while (t_ls)
	{
		// test_name_disp(t_ls->data, m, md, (size_t[]){(size_t)depth, mlen});
		test_res_disp(t_ls->data, m, md, (size_t[2]){(size_t)depth, mlen});
		t_ls = t_ls->next;
	}
}
