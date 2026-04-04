/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tassert_str.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 21:50:05 by bgoulard          #+#    #+#             */
/*   Updated: 2026/03/31 21:50:05 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tc_capture.h"
#include <stdio.h>
#include <unistd.h>
#define CTESTS_INTERNAL_TESTS
#include "include/tc_assert.h"

int	tcit_streq(void)
{
	t_capture	*cap;

	cap = tc_capture_create(STDOUT_FILENO);
	tc_assert_str_eq("a", "b", "1\n");
	tc_assert_str_eq("a", "a", "2\n");
	tc_assert_str_eq("a", "b", NULL);
	tc_assert_str_eq("a", "a", NULL);
	if (tc_capture_compare(cap, "1\n") != 0)
		return (tc_capture_destroy(cap), 1);
	return (tc_capture_destroy(cap), 0);
}

int	tcit_strne(void)
{
	t_capture	*cap;

	cap = tc_capture_create(STDOUT_FILENO);
	tc_assert_str_neq("a", "b", "1\n");
	tc_assert_str_neq("a", "a", "2\n");
	tc_assert_str_neq("a", "b", NULL);
	tc_assert_str_neq("a", "a", NULL);
	if (tc_capture_compare(cap, "2\n") != 0)
		return (tc_capture_destroy(cap), 1);
	return (tc_capture_destroy(cap), 0);
}
