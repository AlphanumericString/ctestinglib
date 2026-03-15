/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_display_tests_numbers.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 16:25:35 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/14 00:31:59 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CTESTS_INTERNAL
#include "tc_modules.h"
#include "tc_utils.h"

static void	loc_mdisplay_percent_txt_and_json(int t_t, int t_p,
				t_display_mode md)
{
	const char	*str_color[] = {KO_COLOR, OK_COLOR};

	if ((md & TC_FMT_MSK) == TC_FMT_JSN)
	{
		if (!t_t)
			return (tci_print_string("\"percentage\":\"NaN%\""), (void)0);
		tci_print_string("\"percentage\":\"");
		tci_print_flt((double)t_p / t_t * 100);
		return (tci_print_string("%\""), (void)0);
	}
	if (!t_t)
		return (tci_print_string("NaN%"), (void)0);
	tci_print_string("(");
	if ((md & TC_PRTY_MSK) == TC_PRTY_COLOR)
		tci_print_string(str_color[t_p == t_t]);
	tci_print_flt((double)t_p / t_t * 100);
	if ((md & TC_PRTY_MSK) == TC_PRTY_COLOR)
		tci_print_string("\033[0m" "%)");
	return ((void)0);
}

void	tci_mdisplay_sum_percent(int t_t, int t_p, t_display_mode md)
{
	if ((md & TC_FMT_MSK) == TC_FMT_JSN
		|| (md & TC_FMT_MSK) == TC_FMT_TXT)
		return (loc_mdisplay_percent_txt_and_json(t_t, t_p, md));
	tci_print_string("Unsupported format for percentage display\n");
}

static void	loc_mdisplay_nb_txt_and_json(int t_t, int t_p, t_display_mode md)
{
	const char	*str_color[] = {KO_COLOR, OK_COLOR};

	if ((md & TC_FMT_MSK) == TC_FMT_JSN)
	{
		tci_print_string("\"nb_tests_passed\":\"");
		tci_print_nb(t_p);
		tci_print_string("\",\"nb_tests_total\":\"");
		tci_print_nb(t_t);
		return (tci_print_string("\""), (void)0);
	}
	if (!t_t)
		return (tci_print_string("empty"), (void)0);
	if ((md & TC_PRTY_MSK) == TC_PRTY_COLOR)
		tci_print_string(str_color[t_p == t_t]);
	tci_print_nb(t_p);
	if ((md & TC_PRTY_MSK) == TC_PRTY_COLOR)
		tci_print_string("\033[0m");
	return (tci_print_string(" / "), tci_print_nb(t_t), (void)0);
}

void	tci_mdisplay_sum_nb(int t_t, int t_p, t_display_mode md)
{
	if ((md & TC_FMT_MSK) == TC_FMT_JSN || (md & TC_FMT_MSK) == TC_FMT_TXT)
		return (loc_mdisplay_nb_txt_and_json(t_t, t_p, md));
	tci_print_string("Unsupported format for percentage display\n");
}
