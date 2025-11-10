/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_testssetups.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 21:02:56 by bgoulard          #+#    #+#             */
/*   Updated: 2025/10/31 21:02:56 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CTESTS_INTERNAL_TESTS
#include "tc_modules.h"

#define TCMODDESCS "Check on handling tests functionalities"

t_module	*create_tests_tests(void)
{
	t_module		*tests_tests;

	tc_module_init(&tests_tests, "Tests", TCMODDESCS);
	tc_module_taddf(tests_tests, tcit_tadd, "add + addf");
	tc_module_taddf(tests_tests, tcit_crash_catcher, "crash catcher");
	tc_module_taddf(tests_tests, tcit_tordering, "ordering");
	tc_module_taddf(tests_tests, tcit_return_check, "return value checking");
	return (tests_tests);
}

#undef TCMODDESCS
