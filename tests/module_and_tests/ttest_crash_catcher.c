#include <stdlib.h>
#include <signal.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_modules.h"

int ctit_crash_catcher(void)
{
	t_module *t;
	int		  ret;

	ct_module_init(&t, "dummy", "dummy for tests");
	ct_module_taddf(t, dummy_sigsegv, "crash_test");
	ret = ct_module_run(t);
	if (ret != EXIT_SUCCESS)
		return (ct_module_destroy(t), 1);
	if (t->tests_passed != 0 || t->tests_count != 1)
		return (ct_module_destroy(t), 2);
	if (((t_test *)t->tests_list->data)->return_value != (SIGSEGV + 128))
		return (ct_module_destroy(t), 3);
	return (ct_module_destroy(t), EXIT_SUCCESS);
}
