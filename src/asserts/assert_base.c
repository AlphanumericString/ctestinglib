/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assert_base.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 09:39:41 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/01 09:39:41 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#define CTESTS_INTERNAL
#include "tc_assert.h"

int	tc_assert_true_v(bool condition, const char *err_msg, va_list args)
{
	if (!condition)
	{
		if (err_msg)
			vprintf(err_msg, args);
		return (1);
	}
	return (0);
}

int	tc_assert_false_v(bool condition, const char *err_msg, va_list args)
{
	if (condition)
	{
		if (err_msg)
			vprintf(err_msg, args);
		return (1);
	}
	return (0);
}
