/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcit_strlen.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 10:12:20 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/05 10:12:20 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#define CTESTS_INTERNAL_TESTS
#include "tc_utils.h"

int	tcit_strlen(void)
{
	if (tc_strlen("") != 0)
		return (1);
	if (tc_strlen("Hello") != 5)
		return (2);
	if (tc_strlen("42 is the answer") != 16)
		return (3);
	if (tc_strlen("A longer string with multiple words.") != 36)
		return (4);
	return (EXIT_SUCCESS);
}
