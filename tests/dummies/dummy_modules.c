/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dummy_modules.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 14:14:13 by bgoulard          #+#    #+#             */
/*   Updated: 2025/09/26 14:14:13 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CTESTS_INTERNAL_TESTS
#include "tc_modules.h"

t_module	*create_string_tests(void)
{
	t_module	*str;

	tc_module_init(&str, "str", "tests for dummy string module");
	tc_module_taddf(str, dummy_success_test, "ss1");
	return (str);
}

t_module	*create_list_tests(void)
{
	t_module	*list;

	tc_module_init(&list, "list", "tests for dummy list module");
	tc_module_taddf(list, dummy_fail_test, "lf1");
	tc_module_taddf(list, dummy_success_test, "ls1");
	tc_module_taddf(list, dummy_success_test, "ls2");
	tc_module_taddf(list, dummy_success_test, "ls3");
	tc_module_taddf(list, dummy_sigsegv, "lc1");
	return (list);
}
