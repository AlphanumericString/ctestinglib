/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_display_chain.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 16:17:33 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/09 16:17:33 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CTESTS_INTERNAL
#include "tc_modules.h"
#include "tc_utils.h"

void	tci_mdisplay_chain(const t_module *m)
{
	if (m->parent)
		tci_mdisplay_chain(m->parent);
	tci_print_string(m->name);
	tci_print_string(":");
}

