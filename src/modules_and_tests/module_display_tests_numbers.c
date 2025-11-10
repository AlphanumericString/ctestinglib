/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_display_tests_numbers.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 16:25:35 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/09 16:25:35 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CTESTS_INTERNAL
#include "tc_modules.h"
#include "tc_utils.h"

void	tci_mdisplay_sum_percent(int t_t, int t_p, t_display_mode md)
{
	const char	*str_color[] = {KO_COLOR, OK_COLOR, "\033[0m"};

	if ((md & TC_DM_SUMMARY_MASK) != TC_DM_SUMPERCENT)
		return ;
	if (!t_t)
		return (tci_print_string("NaN%"), (void)0);
	tci_print_string("(");
	tci_print_string(str_color[t_p == t_t]);
	tci_print_flt((double)t_p / t_t * 100);
	tci_print_string(str_color[2]);
	tci_print_string("%)");
	return ;
}

void	tci_mdisplay_sum_nb(int t_t, int t_p, t_display_mode md)
{
	const char	*str_color[] = {KO_COLOR, OK_COLOR};
	const char	*reset_color = "\033[0m";

	if ((md & TC_DM_SUMMARY_MASK) != TC_DM_SUMARY)
		return ;
	if (!t_t)
		return (tci_print_string("empty"), (void)0);
	tci_print_string(str_color[t_p == t_t]),
	tci_print_nb(t_p);
	tci_print_string(reset_color);
	tci_print_string(" / ");
	tci_print_nb(t_t);
	return ;
}

