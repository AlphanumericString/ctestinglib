/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 21:24:40 by bgoulard          #+#    #+#             */
/*   Updated: 2025/09/16 21:24:40 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define INTERNAL_TESTINGLIB
#include "tests_fxtr.h"

#include <stddef.h>

static t_list	*grab_min(t_list *lst)
{
	t_list	*min;
	size_t	len;

	min = NULL;
	while (lst)
	{
		if (!min || _cmp_alpha(((t_test *)min->data)->name,
				((t_test *)lst->data)->name) > 0)
			min = lst;
		lst = lst->next;
	}
	return (min);
}

void	sort_testslist(t_list *t_ls)
{
	t_list	*min;
	t_test	*data;

	min = NULL;
	while (t_ls)
	{
		min = grab_min(t_ls);
		if (min && min != t_ls)
		{
			data = t_ls->data;
			t_ls->data = min->data;
			min->data = data;
		}
		t_ls = t_ls->next;
		min = NULL;
	}
}
