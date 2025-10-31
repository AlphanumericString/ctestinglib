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

#define INTERNAL_TESTINGLIB
#define INTERNAL_TESTINGLIB_TESTS
#include "tests_fxtr.h"

t_module	*create_modules_tests(void)
{
	t_module		*module_tests;

	module_tests = safealloc(sizeof(*module_tests));
	if (!module_tests)
		return (NULL);
	init_module(module_tests, "Modules", "Check on handling modules functionalities");
	add_test_f(module_tests, tmodule_add, "add submodules");
	add_test_f(module_tests, tmodule_nb_tests, "tests : nb tests");
	add_test_f(module_tests, tmodule_nb_passed, "tests counts : passed count calculation");
	add_test_f(module_tests, tmodule_ordering, "sorting");
	add_test_f(module_tests, tmodule_run, "running modules");
	add_test_f(module_tests, tmodule_init, "initializing modules");
	return (module_tests);
}
