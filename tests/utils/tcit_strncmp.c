/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcit_strncmp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 10:12:46 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/05 10:12:46 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#define CTESTS_INTERNAL_TESTS
#include "tc_utils.h"

int	tcit_strncmp(void)
{
	if (tc_strncmp("Hello", "Hello", 5) != 0
		|| tc_strncmp("Hello", "Hell", 4) != 0)
		return (1);
	if (tc_strncmp("Hello", "Hell", 5) <= 0)
		return (2);
	if (tc_strncmp("Apple", "Banana", 3) >= 0)
		return (3);
	if (tc_strncmp("Short", "Shorter", 10) >= 0)
		return (4);
	if (tc_strncmp("Same", "Same", 10) != 0)
		return (5);
	return (EXIT_SUCCESS);
}
