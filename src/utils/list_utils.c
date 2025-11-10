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

#define CTESTS_INTERNAL
#include "tc_structs.h"
#include "tc_utils.h"
#include "tc_memory.h"

// lst //
void	tci_list_foreach(const t_list *lst, void (*f)(void *data))
{
	if (!lst || !f)
		return ;
	while (lst)
	{
		f(lst->data);
		lst = lst->next;
	}
}

void	tci_list_push(t_list **lst, void *data)
{
	t_list	*leaf;

	leaf = safealloc(sizeof(*leaf));
	if (!leaf)
		return ;
	tc_memset(leaf, 0, sizeof(*leaf));
	leaf->data = data;
	leaf->next = *lst;
	*lst = leaf;
}

// pop
void	*tci_list_pop(t_list **lst)
{
	t_list	*temp;
	void	*data;

	if (!lst || !*lst)
		return (NULL);
	temp = *lst;
	data = temp->data;
	*lst = (*lst)->next;
	safefree(temp);
	return (data);
}
