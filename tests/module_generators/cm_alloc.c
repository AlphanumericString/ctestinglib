#define CTESTS_INTERNAL_TESTS
#include "ct_internal.h"

t_module *create_alloc_tests(void)
{
	t_module *re;

	ct_module_init(&re, "alloc", "desc");
	if (!re)
		return (NULL);
	ct_module_taddf(re, ctit_cleanup, "auto clenaup test");
	ct_module_taddf(re, ctit_count, "allocations count");
	ct_module_taddf(re, ctit_fail, "failloc usage test");
	ct_module_taddf(re, ctit_alloc_variants, "calloc and realloc wrappers");
	ct_module_taddf(re, ctit_free, "free wrapper and cleanup list");
	return (re);
}
