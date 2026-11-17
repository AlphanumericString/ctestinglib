#define CTESTS_INTERNAL
#include "ct_internal.h"

/// @brief Set the setup hook of a module.
/// @details The hook is called once before the module's tests are run, after
/// every submodule has been run. With FORK_TESTS it runs in the parent process,
/// so the tests inherit the state it prepared.
/// @param module Module to configure, NULL is accepted.
/// @param setup Function called before the tests, or NULL to remove the hook.
void ct_module_set_setup(t_module *module, void (*setup)(void))
{
	if (!module)
		return;
	module->setup_function = setup;
}

/// @brief Set the teardown hook of a module.
/// @details The hook is called once after the module's last test, whether the
/// tests passed or not. With FORK_TESTS it runs in the parent process, so it
/// can not observe the changes a test made to its own memory.
/// @param module Module to configure, NULL is accepted.
/// @param teardown Function called after the tests, or NULL to remove the
/// hook.
void ct_module_set_teardown(t_module *module, void (*teardown)(void))
{
	if (!module)
		return;
	module->teardown_function = teardown;
}
