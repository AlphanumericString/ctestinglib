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

#define INTERNAL_TESTINGLIB
#include "tests_fxtr.h"

#include <sys/wait.h>

static void	test_res_disp(t_test *t, t_module *m, t_display_mode md)
{
	if (!(md & TESTS_ALL || t->return_value != EXIT_SUCCESS
			|| !WIFEXITED(t->return_value) || WIFSIGNALED(t->return_value)))
		return ;
	if (WIFEXITED(t->return_value)
		&& WEXITSTATUS(t->return_value) == EXIT_SUCCESS)
		return ((void)print_string("\033[32m" "OK" "\033[0m" "\n"));
	print_string("\033[31m" "KO" "\033[0m");
	print_string("\t");
	if (WIFSIGNALED(t->return_value))
	{
		print_string("Signal: ");
		print_nb(WTERMSIG(t->return_value));
		if (WCOREDUMP(t->return_value))
			print_string(" (core dumped)");
	}
	else
		print_nb(WEXITSTATUS(t->return_value));
	print_string("\n");
}

static void	test_name_disp(t_test *t, t_module *m, t_display_mode md,
				size_t *pass)
{
	size_t	len;
	size_t	mlen;
	int		depth;

	mlen = pass[1];
	depth = (int)pass[0];
	if (!(md & TESTS_ALL || t->return_value != EXIT_SUCCESS))
		return ;
	if (md & (TM_SUM_ALL | TM_INF_ALL) && !(md & TM_NONE))
		print_padding(depth + 1);
	if ((md & TESTS_SILENT) == 0)
	{
		if (md & TESTS_FULLNAME)
			display_module_chain(m);
		print_string(t->name);
		len = 0;
		while (t->name[len])
			len++;
		mlen -= len;
		while (mlen--)
			print_string(" ");
	}
}

static size_t	max_test_name_len(t_module *m)
{
	t_list	*tl;
	t_test	*t;
	size_t	len;
	size_t	len_2;

	len = 0;
	tl = m->tests_list;
	while (tl)
	{
		t = tl->data;
		len_2 = 0;
		while (t && t->name && t->name[len_2])
			len_2++;
		if (len_2 > len)
			len = len_2;
		tl = tl->next;
	}
	return (len);
}

void	_tests_display(t_module *m, t_display_mode md, int depth)
{
	t_list	*t_ls;
	size_t	mlen;

	t_ls = m->tests_list;
	mlen = max_test_name_len(m) + 4;
	while (t_ls)
	{
		test_name_disp(t_ls->data, m, md, (size_t[]){(size_t)depth, mlen});
		test_res_disp(t_ls->data, m, md);
		t_ls = t_ls->next;
	}
}
