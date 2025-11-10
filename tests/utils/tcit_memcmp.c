/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcit_memcmp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 15:07:25 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/03 15:07:25 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#define CTESTS_INTERNAL_TESTS
#include "tc_utils.h"

int	tcit_memcmp(void)
{
	const char	str_digit1[] = "1234567\0009";
	const char	str_digit2[] = "1234567\0009";
	const char	str_char[] = "1234567\000a";

	if (tc_memcmp(str_digit1, str_digit2, 10) != 0)
		return (1);
	if (tc_memcmp(str_digit1, str_char, 10) >= 0)
		return (2);
	if (tc_memcmp(str_char, str_digit1, 10) <= 0)
		return (3);
	if (tc_memcmp(str_digit1, str_digit2, 0) != 0
		|| tc_memcmp(str_digit1, str_digit1, 999) != 0)
		return (4);
	if (tc_memcmp(NULL, str_digit2, 12) >= 0
		|| tc_memcmp(str_digit1, NULL, 12) <= 0)
		return (5);
	return (EXIT_SUCCESS);
}
