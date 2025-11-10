/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_results.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:26:09 by bgoulard          #+#    #+#             */
/*   Updated: 2025/08/08 18:26:09 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CTESTS_INTERNAL
#include "tc_modules.h"

static void	inner_display_results(const t_module *m, t_display_mode md, int depth)
{
	const int	dp = depth;
	t_list		*md_l;

	md_l = m->submodules_list;
	tci_mdisplay_fwrd(m, md, depth);
	while (md_l)
	{
		inner_display_results(md_l->data, md, dp + 1);
		md_l = md_l->next;
	}
	tci_mdisplay_tests(m, md, depth);
	tci_mdisplay_bwrd(m, md, depth);
}

void	tc_module_display(const t_module *module, t_display_mode mode)
{
	const int	depth = 0;

	if (mode == 0)
		mode = TC_DISPLAY_DEFAULT;
	inner_display_results(module, mode, depth);
}
