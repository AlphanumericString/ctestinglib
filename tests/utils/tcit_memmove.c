/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcit_memmove.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 15:08:27 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/03 15:08:27 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#define CTESTS_INTERNAL_TESTS
#include "tc_utils.h"

int	tcit_memmove(void)
{
	char		str[10];
	const char	str3[] = "1234567a9";

	tc_memcpy(str, "123456789", 10);
	tc_memmove(str, "abc", 3);
	if (tc_memcmp(str, "abc456789", 10) != 0)
		return (1);
	tc_memmove(str, str3, 10);
	if (tc_memcmp(str, str3, 10) != 0)
		return (2);
	tc_memmove(str, str + 2, 5);
	if (tc_memcmp(str, "3456767a9", 10) != 0)
		return (3);
	tc_memmove(str + 2, str, 5);
	if (tc_memcmp(str, "3434567a9", 10) != 0)
		return (4);
	if (tc_memmove(str, str, 999) != str || tc_memmove(NULL, str, 99) != NULL
		|| tc_memmove(str, NULL, 99) != str || tc_memmove(str, str, 0) != str)
		return (5);
	return (EXIT_SUCCESS);
}
