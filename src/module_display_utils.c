/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_display_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 19:03:32 by bgoulard          #+#    #+#             */
/*   Updated: 2025/08/24 19:03:32 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define INTERNAL_TESTINGLIB
#include "tests_fxtr.h"

void	module_sbm(t_list	*sbm, t_display_mode md)
{
	const char	*str_color[] = {"\033[31m", "\033[32m", "\033[0m"};
	int			t_t;
	int			t_s;

	while (md & TM_PRINT_SBM && sbm)
	{
		t_t = _collect_sbm_t_total(sbm->data);
		t_s = _collect_sbm_t_success(sbm->data);
		print_string("[");
		print_string(((t_module *)sbm->data)->name);
		print_string(":");
		print_string(str_color[t_s == t_t]);
		print_nb(t_s);
		print_string("/");
		print_nb(t_t);
		print_string(str_color[2]);
		print_string("]");
		sbm = sbm->next;
	}
}

void	module_sum_percent(int t_t, int t_p, t_display_mode md)
{
	const char	*str_color[] = {"\033[31m", "\033[32m", "\033[0m"};

	if (md & TM_SUM_PERCENT)
	{
		if (!t_t && !(md & TM_SUM_NB))
			print_string("NaN%");
		else if (!t_t)
			;
		else
		{
			print_string("(");
			print_string(str_color[t_p == t_t]);
			print_flt((double)t_p / t_t * 100);
			print_string(str_color[2]);
			print_string("%)");
		}
	}
}

void	module_sum_nb(int t_t, int t_p, t_display_mode md)
{
	const char	*str_color[] = {"\033[31m", "\033[32m", "\033[0m"};

	if (!(md & TM_SUM_NB))
		return ;
	if (t_t)
		(print_string(str_color[t_p == t_t]),
			print_nb(t_p), print_string(str_color[2]),
			print_string(" / "), print_nb(t_t));
	else
		print_string("empty");
}
