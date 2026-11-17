#define CTESTS_INTERNAL
#include "ct_internal.h"

/// @brief Enable or disable automatic memory cleanup for a module's tests.
/// @details When it is enabled, allocations made through the wrapped malloc(),
/// calloc() and realloc() while the module's tests run are tracked by the
/// runner and released after the last test instead of being freed right away
/// by free(). The setting is disabled by default and does not apply to the
/// module's submodules.
/// @param module Module to configure, NULL is accepted.
/// @param enabled true to enable automatic cleanup, false to disable it.
void ct_module_set_auto_cleanup(t_module *module, bool enabled)
{
	if (!module)
		return;
	module->auto_cleanup = enabled;
}

/// @brief Get the automatic memory cleanup setting of a module.
/// @param module Module to query, may be NULL.
/// @return true when automatic cleanup is enabled for @p module, false
/// otherwise or when @p module is NULL.
bool ct_module_get_auto_cleanup(const t_module *module)
{
	if (!module)
		return (false);
	return (module->auto_cleanup);
}
