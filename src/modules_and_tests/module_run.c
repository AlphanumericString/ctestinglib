#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define CTESTS_INTERNAL
#include "ct_internal.h"

static int run_module_inner(t_module *module);

static int run_submodules(t_module *module)
{
	int		ret;
	t_list *mod;

	ret = EXIT_SUCCESS;
	mod = module->submodules_list;
	while (mod)
	{
		if (run_module_inner(mod->data) == EXIT_SUCCESS)
			module->module_passed++;
		else
			ret = EXIT_FAILURE;
		mod = mod->next;
	}
	return (ret);
}

static int run_module_inner(t_module *module)
{
	int	 ret;
	bool wrapped;

	wrapped = module->hooks_wrap_submodules;
	if (wrapped == true && cti_module_setup(module) != EXIT_SUCCESS)
	{
		cti_module_teardown(module);
		return (EXIT_FAILURE);
	}
	ret = run_submodules(module);
	if (wrapped == true)
	{
		if (cti_module_run_tests(module) != EXIT_SUCCESS)
			ret = EXIT_FAILURE;
		if (cti_module_teardown(module) != EXIT_SUCCESS)
			ret = EXIT_FAILURE;
	}
	else if (ct_module_run_self_tests(module) != EXIT_SUCCESS)
		ret = EXIT_FAILURE;
	return (ret);
}

/// @brief Run a module and all of its submodules.
/// @details Each submodule is run first, then the module's own tests are run
/// by ct_module_run_self_tests(). When the module's hooks wrap its subtree
/// (see ct_module_set_hooks_wrap_submodules()), setup runs before the
/// submodules and teardown after the module's own tests instead. Tests always
/// run in a forked child, so a crashing test does not stop the whole run.
/// @param module Module tree to run, may be NULL.
/// @return EXIT_SUCCESS when every test passed, EXIT_FAILURE otherwise.
int ct_module_run(t_module *module)
{
	if (!module)
		return (EXIT_FAILURE);
	if (run_module_inner(module) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
