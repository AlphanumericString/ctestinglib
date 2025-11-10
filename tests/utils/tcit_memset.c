/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcit_memset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 15:08:07 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/03 15:08:07 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#define CTESTS_INTERNAL_TESTS
#include "tc_utils.h"

int	tcit_memset(void)
{
	char	str[10];
	char	str2[10];

	tc_memcpy(str, "123456789", 10);
	tc_memcpy(str2, "123456789", 10);
	tc_memset(str, 'a', 10);
	if (tc_memcmp(str, "aaaaaaaaaa", 10) != 0)
		return (1);
	tc_memset(str, 'b', 0);
	if (tc_memcmp(str, "aaaaaaaaaa", 10) != 0)
		return (2);
	tc_memset(str, 'c', 5);
	if (tc_memcmp(str, "cccccaaaaa", 10) != 0)
		return (3);
	return (EXIT_SUCCESS);
}
