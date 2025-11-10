/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_modulesetups.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 08:51:28 by bgoulard          #+#    #+#             */
/*   Updated: 2025/10/28 08:51:28 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CTESTS_INTERNAL_TESTS
#include "tc_modules.h"

#define TCMODDESCS "Check on handling modules functionalities"

t_module	*create_modules_tests(void)
{
	t_module		*module_tests;

	tc_module_init(&module_tests, "Modules", TCMODDESCS);
	tc_module_taddf(module_tests, tcit_add, "add submodules");
	tc_module_taddf(module_tests, tcit_nb_tests, "nb tests");
	tc_module_taddf(module_tests, tcit_nb_passed, "passed count");
	tc_module_taddf(module_tests, tcit_ordering, "sorting");
	tc_module_taddf(module_tests, tcit_run, "running modules");
	tc_module_taddf(module_tests, tcit_init, "initializing modules");
	return (module_tests);
}

#undef TCMODDESCS
