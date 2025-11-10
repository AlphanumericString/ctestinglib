/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assert_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 14:45:10 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/01 14:45:10 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CTESTS_INTERNAL
#include "tc_utils.h"
#include "tc_assert.h"

int	tc_assert_str_eq(const char *s1, const char *s2, const char *err_msg, ...)
{
	va_list	args;
	int		ret;

	va_start(args, err_msg);
	ret = tc_assert_true_v(tc_strncmp(s1, s2, -1) == 0, err_msg, args);
	va_end(args);
	return (ret);
}

int	tc_assert_str_neq(const char *s1, const char *s2, const char *err_msg, ...)
{
	va_list	args;
	int		ret;

	va_start(args, err_msg);
	ret = tc_assert_true_v(tc_strncmp(s1, s2, -1) != 0, err_msg, args);
	va_end(args);
	return (ret);
}
