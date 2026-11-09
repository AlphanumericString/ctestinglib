#define CTESTS_INTERNAL_TESTS
#include "ct_modules.h"

t_module	*create_quiet_dummy(void)
{
	t_module	*str;

	ct_module_init(&str, "str", "tests for dummy string module");
	ct_module_taddf(str, dummy_success_test, "ss1");
	return (str);
}

t_module	*create_dummy_module(void)
{
	t_module	*list;

	ct_module_init(&list, "list", "tests for dummy list module");
	ct_module_taddf(list, dummy_fail_test, "lf1");
	ct_module_taddf(list, dummy_success_test, "ls1");
	ct_module_taddf(list, dummy_success_test, "ls2");
	ct_module_taddf(list, dummy_success_test, "ls3");
	ct_module_taddf(list, dummy_sigsegv, "lc1");
	return (list);
}
