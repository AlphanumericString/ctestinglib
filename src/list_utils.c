/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:10:43 by bgoulard          #+#    #+#             */
/*   Updated: 2025/08/08 18:10:43 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define INTERNAL_TESTINGLIB
#include "tests_fxtr.h"
#include "memutils.h"

// lst //
void	list_foreach(const t_list *lst, void (*f)(void *data))
{
	if (!lst || !f)
		return ;
	while (lst)
	{
		f(lst->data);
		lst = lst->next;
	}
}

void	list_push(t_list **lst, void *data)
{
	t_list	*leaf;

	leaf = safealloc(sizeof(*leaf));
	if (!leaf)
		return ;
	self_memset(leaf, 0, sizeof(*leaf));
	leaf->data = data;
	leaf->next = *lst;
	*lst = leaf;
}
