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

#define CTESTS_INTERNAL
#include "tc_modules.h"
#include "tc_utils.h"

void	tci_mdisplay_sbm(const t_list *sbm, t_display_mode md)
{
	const char	*str_color[] = {KO_COLOR, OK_COLOR, "\033[0m"};
	int			t_t;
	int			t_s;

	while ((md & TC_DM_SUMMARY_MASK) == TC_DM_SILENT && sbm)
	{
		t_t = tci_collect_sbm_t_total(sbm->data);
		t_s = tci_collect_sbm_t_success(sbm->data);
		tci_print_string("[");
		tci_print_string(((t_module *)sbm->data)->name);
		tci_print_string(":");
		tci_print_string(str_color[t_s == t_t]);
		tci_print_nb(t_s);
		tci_print_string("/");
		tci_print_nb(t_t);
		tci_print_string(str_color[2]);
		tci_print_string("]");
		sbm = sbm->next;
	}
}
