#define CTESTS_INTERNAL
#include "ct_modules.h"

void ct_module_set_timeout(t_module *module, unsigned int seconds)
{
	if (!module)
		return;
	module->timeout = seconds;
}

unsigned int ct_module_get_timeout(const t_module *module)
{
	if (!module)
		return (0);
	return (module->timeout);
}

void ct_test_set_timeout(t_test *test, unsigned int seconds)
{
	if (!test)
		return;
	test->timeout = seconds;
}

unsigned int ct_test_get_timeout(const t_test *test)
{
	if (!test)
		return (0);
	return (test->timeout);
}
