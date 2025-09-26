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

#include "../include/tests_fxtr.h"

int	_collect_sbm_t_success(t_module	*m)
{
	t_list	*md_l;
	int		nb;

	nb = 0;
	md_l = m->submodules_list;
	while (md_l)
	{
		nb += _collect_sbm_t_success(md_l->data);
		md_l = md_l->next;
	}
	return (nb + m->tests_passed);
}

int	_collect_sbm_t_total(t_module *m)
{
	t_list	*md_l;
	int		nb;

	nb = 0;
	md_l = m->submodules_list;
	while (md_l)
	{
		nb += _collect_sbm_t_total(md_l->data);
		md_l = md_l->next;
	}
	return (nb + m->tests_count);
}
