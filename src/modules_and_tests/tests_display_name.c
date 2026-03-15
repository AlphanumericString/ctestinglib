/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_display_name.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 00:06:13 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/14 00:06:13 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CTESTS_INTERNAL
#include "tc_modules.h"
#include "tc_utils.h"

static void	tci_mdisplay_chain(const t_module *m)
{
	if (m->parent)
		tci_mdisplay_chain(m->parent);
	tci_print_string(m->name);
	tci_print_string(":");
}

static void	loc_hdl_fmts(int depth, t_display_mode md)
{
	if ((md & TC_DM_SUMMSK) != TC_DM_SUMSILENT
		&& (md & TC_FMT_MSK) == TC_FMT_TXT)
		tci_print_charn(depth + 1, '\t');
	if ((md & TC_FMT_MSK) == TC_FMT_JSN)
		tci_print_string("{\"test_name\":\"");
}

static void	loc_cls_fmts(const t_test *t, size_t ml, t_display_mode md)
{
	if ((md & TC_FMT_MSK) == TC_FMT_JSN)
		return (tci_print_string("\","), (void)0);
	else if ((md & TC_FMT_MSK) == TC_FMT_TXT)
		tci_print_charn(ml - tc_strlen(t->name), ' ');
}

void	tci_td_name(const t_test *t, const t_module *m, t_display_mode md,
			size_t *pass)
{
	const size_t	mlen = pass[1];
	const int		depth = (int)pass[0];

	loc_hdl_fmts(depth, md);
	if ((md & TC_DT_NAMEMSK) == TC_DT_FULLNAME)
		tci_mdisplay_chain(m);
	tci_print_string(t->name);
	loc_cls_fmts(t, mlen, md);
}
