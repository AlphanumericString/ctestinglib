#define CTESTS_INTERNAL
#include "ct_internal.h"

/// @brief Set the default timeout of a module's tests.
/// @param module Module to configure, NULL is accepted.
/// @param seconds Timeout in seconds; 0 means no module-level timeout.
void ct_module_set_timeout(t_module *module, unsigned int seconds)
{
	if (!module)
		return;
	module->timeout = seconds;
}

/// @brief Get the default timeout of a module's tests.
/// @param module Module to query, may be NULL.
/// @return The module timeout in seconds, or 0 when @p module is NULL or has
/// no timeout.
unsigned int ct_module_get_timeout(const t_module *module)
{
	if (!module)
		return (0);
	return (module->timeout);
}

/// @brief Set the timeout of a single test.
/// @param test Test to configure, NULL is accepted.
/// @param seconds Timeout in seconds; 0 falls back to the module timeout and
/// then to CT_DEFAULT_TIMEOUT.
void ct_test_set_timeout(t_test *test, unsigned int seconds)
{
	if (!test)
		return;
	test->timeout = seconds;
}

/// @brief Get the timeout of a single test.
/// @param test Test to query, may be NULL.
/// @return The test timeout in seconds, or 0 when @p test is NULL or has no
/// specific timeout.
unsigned int ct_test_get_timeout(const t_test *test)
{
	if (!test)
		return (0);
	return (test->timeout);
}
