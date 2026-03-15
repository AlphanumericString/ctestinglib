/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_display_sbm.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 19:03:32 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/14 00:31:52 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CTESTS_INTERNAL
#include "tc_modules.h"
#include "tc_utils.h"

static void	loc_display_sbm_txt(const t_list *sbm, t_display_mode md)
{
	t_module	*m;
	int			t_t;

	while ((md & TC_DM_SUMMSK) != TC_DM_SUMSILENT && sbm)
	{
		m = sbm->data;
		t_t = tci_collect_sbm_t_total(m);
		sbm = sbm->next;
		tci_print_string("[");
		tci_print_string(m->name);
		tci_print_string(": ");
		if ((md & TC_DSBM_SUMMSK) == TC_DSBM_SUMPCT)
			tci_mdisplay_sum_percent(t_t, tci_collect_sbm_t_success(m), md);
		else
			tci_mdisplay_sum_nb(t_t, tci_collect_sbm_t_success(m), md);
		tci_print_string("]");
	}
	tci_print_string("\n");
}

// json doesn't have one as they "print in place" and dont do a recap
// for each sub module but simple nests them in their parents
void	tci_mdisplay_sbm(const t_list *sbm, t_display_mode md)
{
	if ((md & TC_FMT_MSK) == TC_FMT_TXT)
		return (loc_display_sbm_txt(sbm, md));
	tci_print_string("Unsupported format for submodule display\n");
}
