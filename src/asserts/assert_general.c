/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assert_general.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 09:14:29 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/14 00:31:20 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CTESTS_INTERNAL
#include "tc_assert.h"

int	tc_assert_true(bool condition, const char *err_msg, ...)
{
	va_list	args;
	int		ret;

	va_start(args, err_msg);
	ret = tc_assert_true_v(condition, err_msg, args);
	va_end(args);
	return (ret);
}

int	tc_assert_false(bool condition, const char *err_msg, ...)
{
	va_list	args;
	int		ret;

	va_start(args, err_msg);
	ret = tc_assert_false_v(condition, err_msg, args);
	va_end(args);
	return (ret);
}
