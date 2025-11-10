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

#include "tc_memory.h"
#include <stdio.h>
#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "tc_modules.h"

int	tcit_init(void)
{
	const char	*deflt_names[] = {"Start name", "Module Name"};
	const char	*deflt_dscs[] = {"Start description", "Module description"};
	t_module	*mod_tests;
	t_module	*no_leak;

	mod_tests = NULL;
	tc_module_init(&mod_tests, NULL, deflt_dscs[1]);
	if (mod_tests != NULL)
		return (EXIT_FAILURE);
	tc_module_init(&mod_tests, deflt_names[1], NULL);
	if (mod_tests->name != deflt_names[1] || mod_tests->description != NULL)
		return (EXIT_FAILURE);
	mod_tests->submodules_list = (void *)0xDEADBEEF;
	mod_tests->tests_list = (void *)0xDEADCAFE;
	mod_tests->depth = 42;
	no_leak = mod_tests;
	tc_module_init(&mod_tests, deflt_names[0], deflt_dscs[0]);
	if (mod_tests->submodules_list != NULL || mod_tests->depth != 0
		|| mod_tests->tests_list != NULL || no_leak == mod_tests)
		return (EXIT_FAILURE);
	return (safefree(no_leak), tc_module_destroy(mod_tests), EXIT_SUCCESS);
}
