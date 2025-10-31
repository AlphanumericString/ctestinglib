/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmodule_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 21:55:52 by bgoulard          #+#    #+#             */
/*   Updated: 2025/10/31 21:55:52 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#define INTERNAL_TESTINGLIB
#define INTERNAL_TESTINGLIB_TESTS
#include "tests_fxtr.h"

int	tmodule_init(void)
{
	const char	*deflt_names[] = {"Module Start name", "Module Name"};
	const char	*deflt_dscs[] = {"Module Start description", "Module description"};
	t_module	mod_tests;

	mod_tests = (t_module){0};
	mod_tests.name = deflt_names[0];
	mod_tests.description = deflt_dscs[0];
	init_module(&mod_tests, NULL, deflt_dscs[1]);
	if (mod_tests.name != deflt_names[0] || mod_tests.description != deflt_dscs[0])
		return (EXIT_FAILURE);
	init_module(&mod_tests, deflt_names[1], NULL);
	if (mod_tests.name != deflt_names[1] || mod_tests.description != NULL)
		return (EXIT_FAILURE);
	mod_tests.submodules_list = (void *)0xDEADBEEF;
	mod_tests.tests_list = (void *)0xDEADCAFE;
	mod_tests.depth = 42;
	init_module(&mod_tests, deflt_names[0], deflt_dscs[0]);
	if (mod_tests.submodules_list != NULL || mod_tests.depth != 0
		|| mod_tests.tests_list != NULL)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
