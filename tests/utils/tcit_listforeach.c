/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcit_listforeach.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 13:04:14 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/07 13:04:14 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tc_memory.h"
#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "tc_utils.h"

static void	tcit_lfe_helper(void *data)
{
	int	*val;

	val = (int *)data;
	*val += 1;
}

static int	normal_listforeach(void)
{
	t_list	*lst;
	int		val1;
	int		val2;
	int		val3;

	lst = NULL;
	val1 = 0;
	val2 = 0;
	val3 = 0;
	tci_list_push(&lst, &val1);
	tci_list_push(&lst, &val2);
	tci_list_push(&lst, &val3);
	tci_list_foreach(lst, tcit_lfe_helper);
	if (val1 != 1 || val2 != 1 || val3 != 1)
		return (safefree(lst->next->next), safefree(lst->next),
			safefree(lst), 1);
	return (safefree(lst->next->next), safefree(lst->next),
		safefree(lst), EXIT_SUCCESS);
}

static int	err_check(void)
{
	tci_list_foreach(NULL, tcit_lfe_helper);
	tci_list_foreach(NULL, NULL);
	tci_list_foreach((t_list *)1, NULL);
	return (EXIT_SUCCESS);
}

int	tcit_listforeach(void)
{
	int	ret;

	ret = normal_listforeach();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = err_check();
	if (ret != EXIT_SUCCESS)
		return (ret);
	return (EXIT_SUCCESS);
}
