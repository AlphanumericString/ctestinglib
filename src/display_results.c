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

#define INTERNAL_TESTINGLIB
#include "tests_fxtr.h"

static void	inner_display_results(t_module *m, t_display_mode md, int depth)
{
	const int	dp = depth;
	t_list		*md_l;

	md_l = m->submodules_list;
	module_display_fwrd(m, md, depth);
	while (md_l)
	{
		inner_display_results(md_l->data, md, dp + 1);
		md_l = md_l->next;
	}
	_tests_display(m, md, depth);
	module_display_bwrd(m, md, depth);
}

void	display_results(t_module *module, t_display_mode mode)
{
	const int	depth = 0;

	if (mode == 0)
		mode = DEFAULT;
	inner_display_results(module, mode, depth);
}
