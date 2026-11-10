#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_modules.h"

static int timeout_per_test(void)
{
	t_module *m;
	t_test	 *t;

	ct_module_init(&m, "timeout", "per test timeout");
	ct_module_taddf_timeout(m, dummy_timeout, "hang", 1);
	t = m->tests_list->data;
	if (ct_test_get_timeout(t) != 1)
		return (ct_module_destroy(m), 1);
	ct_module_run(m);
	if (t->timed_out != true || m->tests_passed != 0)
		return (ct_module_destroy(m), 2);
	return (ct_module_destroy(m), EXIT_SUCCESS);
}

static int timeout_per_module(void)
{
	t_module *m;
	t_test	 *t;

	ct_module_init(&m, "timeout", "per module timeout");
	ct_module_set_timeout(m, 1);
	if (ct_module_get_timeout(m) != 1)
		return (ct_module_destroy(m), 1);
	ct_module_taddf(m, dummy_timeout, "hang");
	ct_module_run(m);
	t = m->tests_list->data;
	if (t->timed_out != true || m->tests_passed != 0)
		return (ct_module_destroy(m), 2);
	return (ct_module_destroy(m), EXIT_SUCCESS);
}

static int timeout_test_setter(void)
{
	t_module *m;
	t_test	 *t;

	ct_module_init(&m, "timeout", "per test setter");
	ct_module_taddf(m, dummy_timeout, "hang");
	t = m->tests_list->data;
	ct_test_set_timeout(t, 1);
	if (ct_test_get_timeout(t) != 1)
		return (ct_module_destroy(m), 1);
	ct_module_run(m);
	if (t->timed_out != true || m->tests_passed != 0)
		return (ct_module_destroy(m), 2);
	return (ct_module_destroy(m), EXIT_SUCCESS);
}

static int timeout_no_effect(void)
{
	t_module *m;
	t_test	 *t;

	ct_module_init(&m, "timeout", "timeout does not affect fast tests");
	ct_module_taddf_timeout(m, dummy_success_test, "fast", 5);
	ct_module_run(m);
	t = m->tests_list->data;
	if (t->timed_out != false || m->tests_passed != 1)
		return (ct_module_destroy(m), 1);
	return (ct_module_destroy(m), EXIT_SUCCESS);
}

static int timeout_guards(void)
{
	ct_module_set_timeout(NULL, 1);
	ct_test_set_timeout(NULL, 1);
	if (ct_module_get_timeout(NULL) != 0)
		return (1);
	if (ct_test_get_timeout(NULL) != 0)
		return (2);
	return (EXIT_SUCCESS);
}

int ctit_timeout(void)
{
	int ret;

	ret = timeout_guards();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = timeout_per_test();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = timeout_per_module();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = timeout_test_setter();
	if (ret != EXIT_SUCCESS)
		return (ret);
	return (timeout_no_effect());
}
