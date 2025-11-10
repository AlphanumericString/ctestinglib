/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tc_assert.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 09:18:41 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/01 09:18:41 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TC_ASSERT_H
# define TC_ASSERT_H

# include <stddef.h>
# include <stdbool.h>

# ifdef CTESTS_INTERNAL

#  include <stdarg.h>

int	tc_assert_false_v(bool condition, const char *err_msg, va_list args);
int	tc_assert_true_v(bool condition, const char *err_msg, va_list args);

# endif /* CTESTS_INTERNAL */

int	tc_assert_true(bool condition, const char *err_msg, ...);
int	tc_assert_false(bool condition, const char *err_msg, ...);

int	tc_assert_str_eq(const char *s1, const char *s2, const char *err_msg, ...);
int	tc_assert_str_neq(const char *s1, const char *s2, const char *err_msg, ...);

#endif /* TC_ASSERT_H */
