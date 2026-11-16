#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define CTESTS_INTERNAL
#include "ct_modules.h"

static int run_module_inner(t_module *module)
{
	int		ret;
	t_list *mod;

	mod = module->submodules_list;
	while (mod)
	{
		ret = run_module_inner(mod->data);
		if (ret == EXIT_SUCCESS)
			module->module_passed++;
		mod = mod->next;
	}
	return (ct_module_run_self_tests(module));
}

/// @brief Run a module and all of its submodules.
/// @details Each submodule is run first, then the module's own tests are run
/// by ct_module_run_self_tests(). Tests run in a forked child by default
/// (FORK_TESTS), so a crashing test does not stop the whole run.
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
