#define CTESTS_INTERNAL_TESTS
#include "ct_modules.h"
#include "ct_memory.h"

t_module *create_alloc_tests(void)
{
	t_module *re;

	ct_module_init(&re, "alloc", "desc");
	if (!re)
		return (NULL);
	ct_module_taddf(re, ctit_cleanup, "auto clenaup test");
	ct_module_taddf(re, ctit_count, "allocations count");
	ct_module_taddf(re, ctit_fail, "failloc usage test");
	return (re);
}
