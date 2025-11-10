/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcit_listpop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 13:04:11 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/07 13:04:11 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tc_memory.h"
#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "tc_utils.h"

static int	normal_list_pop(void)
{
	t_list	*lst;
	void	*data;

	lst = NULL;
	if (tci_list_pop(&lst) != NULL)
		return (1);
	tci_list_push(&lst, (void *)1);
	tci_list_push(&lst, (void *)2);
	data = tci_list_pop(&lst);
	if (data != (void *)2)
		return (safefree(lst), 2);
	data = tci_list_pop(&lst);
	if (data != (void *)1)
		return (safefree(lst), 3);
	if (tci_list_pop(&lst) != NULL)
		return (4);
	return (EXIT_SUCCESS);
}

static int	err_check(void)
{
	t_list	*lst;
	void	*data;

	lst = NULL;
	data = tci_list_pop(NULL);
	if (data != NULL)
		return (1);
	data = tci_list_pop(&lst);
	if (data != NULL)
		return (2);
	return (EXIT_SUCCESS);
}

int	tcit_listpop(void)
{
	int	ret;

	ret = normal_list_pop();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = err_check();
	return (ret);
}
