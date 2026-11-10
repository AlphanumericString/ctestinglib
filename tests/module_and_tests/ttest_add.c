#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_modules.h"

static int ttest_add_param_checks(void)
{
	ct_module_tadd(NULL, (t_test *)0xDEADBEEF);
	ct_module_tadd((t_module *)0xDEADBEEF, NULL);
	return (EXIT_SUCCESS);
}

static int ttest_addf(t_module *t)
{
	ct_module_taddf(NULL, dummy_success_test, "ok1");
	ct_module_taddf(t, NULL, "aaaa");
	ct_module_taddf(t, dummy_success_test, NULL);
	if (t->tests_count != 0)
		return (2);
	return (EXIT_SUCCESS);
}

int ctit_tadd(void)
{
	const char *names[] = {"test1", "test2", "test3"};
	t_module   *t;

	ct_module_init(&t, "dummy", "dummy for tests");
	ttest_add_param_checks();
	ttest_addf(t);
	ct_module_taddf(t, dummy_success_test, names[0]);
	if (t->tests_count != 1 && t->tests_list == NULL)
		return (ct_module_destroy(t), 1);
	ct_module_taddf(t, dummy_success_test, names[1]);
	ct_module_taddf(t, dummy_success_test, names[2]);
	if (t->tests_count != 3 && t->tests_list == NULL)
		return (ct_module_destroy(t), 2);
	if (((t_test *)t->tests_list->data)->name != names[2] ||
		((t_test *)t->tests_list->next->data)->name != names[1] ||
		((t_test *)t->tests_list->next->next->data)->name != names[0])
		return (ct_module_destroy(t), 3);
	return (ct_module_destroy(t), EXIT_SUCCESS);
}
