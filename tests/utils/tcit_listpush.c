/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcit_listpush.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 13:03:49 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/07 13:03:49 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tc_memory.h"
#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "tc_utils.h"

int	tcit_listpush(void)
{
	t_list	*lst;

	lst = NULL;
	tci_list_push(&lst, (void *)1);
	if (!lst || lst->data != (void *)1 || lst->next != NULL)
		return (1);
	tci_list_push(&lst, (void *)2);
	if (!lst || lst->data != (void *)2 || !lst->next
		|| lst->next->data != (void *)1 || lst->next->next != NULL)
		return (safefree(lst), 2);
	return (safefree(lst->next), safefree(lst), EXIT_SUCCESS);
}
