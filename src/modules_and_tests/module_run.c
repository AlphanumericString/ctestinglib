/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_run.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:18:13 by bgoulard          #+#    #+#             */
/*   Updated: 2025/08/17 01:36:07 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define CTESTS_INTERNAL
#include "tc_modules.h"

static int	run_module_inner(t_module *module)
{
	int			ret;
	t_list		*mod;

	mod = module->submodules_list;
	while (mod)
	{
		ret = run_module_inner(mod->data);
		if (ret == EXIT_SUCCESS)
			module->module_passed++;
		mod = mod->next;
	}
	return (tc_module_run_self_tests(module));
}

int	tc_module_run(t_module *module)
{
	if (!module)
		return (EXIT_FAILURE);
	if (run_module_inner(module) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
/// TODO: for run_module
/// * Signal handler?
/// * modify t_test to keep pid and run the wait pid afterward
///		[bad, race conditions still apear in the setup shown above]
/// * Add a hook to run a setup/teardown for module/tests?
/// * Add a timeout for each test?
///		[can't do with fork&wait, might be possible to use usleep and kill]
