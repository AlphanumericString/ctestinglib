/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tassert_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 21:47:25 by bgoulard          #+#    #+#             */
/*   Updated: 2026/03/31 21:47:25 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tc_capture.h"
#include <unistd.h>
#define CTESTS_INTERNAL_TESTS
#include "include/tc_assert.h"

#define EXP_TRUE	"24"
#define EXP_FALSE	"13"
#define EXP_PRINT_ARG	"12 - blue - ff\n"

int	tcit_true(void)
{
	t_capture	*cap;
	int			ret;

	ret = 0;
	if (tc_assert_true(false, NULL) == 0 || tc_assert_true(true, NULL) != 0)
		return (1);
	cap = tc_capture_create(STDOUT_FILENO);
	ret += tc_assert_true(true, "1");
	ret += tc_assert_true(false, "2");
	ret += tc_assert_true(true, "%d", 3);
	ret += tc_assert_true(false, "%d", 4);
	ret += tc_assert_true(false, "%d - %s - %x\n", 12, "blue", 0xFF);
	if (tc_capture_compare(cap, EXP_TRUE EXP_PRINT_ARG) != 0 || ret != 3)
		return (tc_capture_destroy(cap), 1);
	return (tc_capture_destroy(cap), 0);
}

int	tcit_false(void)
{
	t_capture	*cap;
	int			ret;

	ret = 0;
	if (tc_assert_false(true, NULL) == 0 || tc_assert_false(false, NULL) != 0)
		return (1);
	cap = tc_capture_create(STDOUT_FILENO);
	ret += tc_assert_false(true, "1");
	ret += tc_assert_false(false, "2");
	ret += tc_assert_false(true, "%d", 3);
	ret += tc_assert_false(false, "%d", 4);
	ret += tc_assert_false(true, "%d - %s - %x\n", 12, "blue", 0xFF);
	if (tc_capture_compare(cap, EXP_FALSE EXP_PRINT_ARG) != 0 || ret != 3)
		return (tc_capture_destroy(cap), 1);
	return (tc_capture_destroy(cap), 0);
}
