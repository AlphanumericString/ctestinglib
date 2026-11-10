#include <stdlib.h>

#define CTESTS_INTERNAL_TESTS
#include "ct_modules.h"

int ctit_run(void)
{
	t_module *t;
	int		  r;

	ct_module_init(&t, "test", "module for tests to run on");
	r = ct_module_run(NULL);
	if (r != EXIT_FAILURE)
		return (ct_module_destroy(t), EXIT_FAILURE);
	r = ct_module_run(t);
	if (r != EXIT_SUCCESS)
		return (ct_module_destroy(t), r);
	ct_module_taddf(t, dummy_success_test, "test1");
	ct_module_taddf(t, dummy_fail_test, "test2");
	r = ct_module_run(t);
	return (ct_module_destroy(t), r);
}
