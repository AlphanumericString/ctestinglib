/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcit_memcpy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 15:08:43 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/03 15:08:43 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#define CTESTS_INTERNAL_TESTS
#include "tc_utils.h"

int	tcit_memcpy(void)
{
	char		str_buffer[10];
	const char	str_digits[] = "1234567\0009";

	if (tc_memcpy(str_buffer, "123456789", 10) != str_buffer
		|| tc_memcpy(str_buffer, "abc", 3) != str_buffer)
		return (1);
	if (tc_memcmp(str_buffer, "abc456789", 10) != 0)
		return (2);
	tc_memcpy(str_buffer, str_digits, 10);
	if (tc_memcmp(str_buffer, str_digits, 10) != 0)
		return (3);
	tc_memcpy(str_buffer, str_buffer, 999);
	if (tc_memcpy(NULL, str_digits, 2) != NULL
		|| tc_memcpy(str_buffer, NULL, 2) != NULL)
		return (4);
	return (EXIT_SUCCESS);
}
