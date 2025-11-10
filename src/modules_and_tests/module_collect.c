/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_collect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 21:19:16 by bgoulard          #+#    #+#             */
/*   Updated: 2025/08/16 21:19:16 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tc_utils.h"
#define CTESTS_INTERNAL
#include "tc_modules.h"

int	tci_collect_sbm_t_success(const t_module	*m)
{
	t_list	*md_l;
	int		nb;

	nb = 0;
	md_l = m->submodules_list;
	while (md_l)
	{
		nb += tci_collect_sbm_t_success(md_l->data);
		md_l = md_l->next;
	}
	return (nb + m->tests_passed);
}

int	tci_collect_sbm_t_total(const t_module *m)
{
	t_list	*md_l;
	int		nb;

	nb = 0;
	md_l = m->submodules_list;
	while (md_l)
	{
		nb += tci_collect_sbm_t_total(md_l->data);
		md_l = md_l->next;
	}
	return (nb + m->tests_count);
}

size_t	tci_mget_longest_name(const t_module *m)
{
	t_list	*tl;
	size_t	i;
	size_t	tmp;

	i = 0;
	tl = m->tests_list;
	while (tl && tl->data)
	{
		tmp = tc_strlen(((const t_test *)tl->data)->name);
		if (tmp > i)
			i = tmp;
		tl = tl->next;
	}
	return (i);
}
