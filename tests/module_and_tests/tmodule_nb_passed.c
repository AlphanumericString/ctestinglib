#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_modules.h"

int	ctit_nb_passed(void)
{
	t_module	*t;

	ct_module_init(&t, "test", "module for tests to run on");
	ct_module_taddf(t, dummy_success_test, "ok1");
	ct_module_run(t);
	if (t->tests_passed != 1)
		return (ct_module_destroy(t), 1);
	ct_module_taddf(t, dummy_fail_test, "ko1");
	ct_module_run(t);
	if (t->tests_passed != 1 && t->tests_count != 2)
		return (ct_module_destroy(t), 2);
	ct_module_taddf(t, dummy_sigsegv, "crash1");
	ct_module_run(t);
	if (t->tests_passed != 1 && t->tests_count != 3)
		return (ct_module_destroy(t), 3);
	return (ct_module_destroy(t), EXIT_SUCCESS);
}
