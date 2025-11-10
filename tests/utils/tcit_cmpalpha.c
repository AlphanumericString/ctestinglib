/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcit_cmpalpha.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:27:57 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/07 15:27:57 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "tc_utils.h"

int	tcit_cmpalpha(void)
{
	if (tci_cmp_alpha("Apple", "Apple") != 0)
		return (1);
	if (tci_cmp_alpha("Apple", "Applf") >= 0)
		return (2);
	if (tci_cmp_alpha("Applf", "Apple") <= 0)
		return (3);
	if (tci_cmp_alpha(NULL, "Apple") >= 0
		|| tci_cmp_alpha("Apple", NULL) <= 0)
		return (4);
	if (tci_cmp_alpha(NULL, NULL) != 0)
		return (5);
	return (EXIT_SUCCESS);
}
