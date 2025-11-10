/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auto_cleanup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 17:13:28 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/01 17:13:28 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CTESTS_INTERNAL
#include "tc_memory.h"
#include "tc_utils.h"

extern void	__libc_free(void *ptr);

static bool	*tci_memclnup_state(void)
{
	static bool	state = false;

	return (&state);
}

static t_list	**cleanup_list(void)
{
	static t_list	*list = NULL;

	return (&list);
}

void	tci_set_memory_cleanup(bool val)
{
	*tci_memclnup_state() = val;
}

bool	tci_get_memory_cleanup(void)
{
	return (*tci_memclnup_state());
}

void	push_to_cleanup(void *ptr)
{
	t_list	**ptr_list;

	ptr_list = cleanup_list();
	return (tci_list_push(ptr_list, ptr));
}

void	do_cleanup(void (*f)(void *))
{
	t_list	*leaf;
	t_list	*nxt;

	if (!f)
		f = __libc_free;
	leaf = *cleanup_list();
	while (leaf)
	{
		f(leaf->data);
		nxt = leaf->next;
		__libc_free(leaf);
		leaf = nxt;
	}
	*cleanup_list() = NULL;
}
