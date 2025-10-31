/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_sort.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 21:28:49 by bgoulard          #+#    #+#             */
/*   Updated: 2025/09/16 21:28:49 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define INTERNAL_TESTINGLIB
#include "tests_fxtr.h"
#include <string.h>

static t_list	*grab_min(t_list *lst)
{
	t_list	*min;

	min = NULL;
	while (lst)
	{
		if (!min || _cmp_alpha(((t_module *)min->data)->name,
				((t_module *)lst->data)->name) > 0)
			min = lst;
		lst = lst->next;
	}
	return (min);
}

void	sort_moduleslist(t_list *m_ls)
{
	t_list		*min;
	t_module	*data;

	min = NULL;
	while (m_ls)
	{
		min = grab_min(m_ls);
		if (min && min != m_ls)
		{
			data = m_ls->data;
			m_ls->data = min->data;
			min->data = data;
		}
		m_ls = m_ls->next;
		min = NULL;
	}
}

void	sort_recursive(t_module *root)
{
	t_list	*tmp;

	if (!root)
		return ;
	if (root->tests_list)
		sort_testslist(root->tests_list);
	if (!root->submodules_list)
		return ;
	sort_moduleslist(root->submodules_list);
	tmp = root->submodules_list;
	while (tmp && tmp->data)
	{
		sort_testslist(((t_module *)tmp->data)->tests_list);
		sort_recursive((t_module *)tmp->data);
		tmp = tmp->next;
	}
}
