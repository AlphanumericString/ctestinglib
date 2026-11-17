#define CTESTS_INTERNAL
#include "ct_internal.h"

/// @brief Set the setup hook of a module.
/// @details The hook is called once before the module's tests are run, after
/// every submodule has been run by default. It runs in the parent process, so
/// the forked tests inherit the state it prepared. Returning false marks the
/// module as failed and skips its tests; the teardown hook still runs.
/// @param module Module to configure, NULL is accepted.
/// @param setup Function called before the tests, or NULL to remove the hook.
void ct_module_set_setup(t_module *module, bool (*setup)(void))
{
	if (!module)
		return;
	module->setup_function = setup;
}

/// @brief Set the teardown hook of a module.
/// @details The hook is called once after the module's last test, whether the
/// tests passed or not. It runs in the parent process, so it can not observe
/// the changes a test made to its own memory. Returning false marks the module
/// as failed.
/// @param module Module to configure, NULL is accepted.
/// @param teardown Function called after the tests, or NULL to remove the
/// hook.
void ct_module_set_teardown(t_module *module, bool (*teardown)(void))
{
	if (!module)
		return;
	module->teardown_function = teardown;
}

/// @brief Make a module's hooks wrap its whole subtree.
/// @details By default the setup hook runs after the submodules, so it only
/// covers the module's own tests. When @p enabled is true, setup runs before
/// the submodules and teardown after the module's own tests, so the same
/// fixture covers the whole subtree. A setup failure then skips the submodules
/// as well; teardown still runs.
/// @note This only changes ct_module_run() and ct_module_run_args():
/// ct_module_run_self_tests() never runs submodules.
/// @param module Module to configure, NULL is accepted.
/// @param enabled true to make the hooks wrap the subtree, false for the
/// default behavior.
void ct_module_set_hooks_wrap_submodules(t_module *module, bool enabled)
{
	if (!module)
		return;
	module->hooks_wrap_submodules = enabled;
}

/// @brief Get the subtree hook setting of a module.
/// @param module Module to query, may be NULL.
/// @return true when the module's hooks wrap its subtree, false otherwise or
/// when @p module is NULL.
bool ct_module_get_hooks_wrap_submodules(const t_module *module)
{
	if (!module)
		return (false);
	return (module->hooks_wrap_submodules);
}
