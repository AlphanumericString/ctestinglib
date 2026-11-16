#include <stdlib.h>
#include <string.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_internal.h"

static int param_expect(void *param)
{
	const int *value;

	value = param;
	if (!value)
		return (EXIT_FAILURE);
	return (*value == 42 ? EXIT_SUCCESS : EXIT_FAILURE);
}

static int param_fail(void *param)
{
	(void)param;
	return (EXIT_FAILURE);
}

static t_test *find_test(t_module *m, const char *name)
{
	t_list *it;

	it = m->tests_list;
	while (it)
	{
		if (strcmp(((t_test *)it->data)->name, name) == 0)
			return (it->data);
		it = it->next;
	}
	return (NULL);
}

static int param_guards(t_module *m, int *value)
{
	if (ct_module_taddf_param(NULL, param_expect, value, "guard") != false)
		return (1);
	if (ct_module_taddf_param(m, NULL, value, "guard") != false)
		return (2);
	if (ct_module_taddf_param(m, param_expect, value, NULL) != false)
		return (3);
	if (m->tests_count != 0)
		return (4);
	return (EXIT_SUCCESS);
}

static int param_check_types(t_module *m, int *value)
{
	t_test *t;

	t = find_test(m, "ok");
	if (!t || t->type != CT_PARAM || t->param != value)
		return (1);
	t = find_test(m, "timed");
	if (!t || t->type != CT_PARAM || t->param != value ||
		ct_test_get_timeout(t) != 3)
		return (2);
	t = find_test(m, "ko");
	if (!t || t->type != CT_PARAM || t->param != NULL)
		return (3);
	t = find_test(m, "pure");
	if (!t || t->type != CT_TPURE || t->test_func.pure != dummy_success_test)
		return (4);
	return (EXIT_SUCCESS);
}

int ctit_tparam(void)
{
	t_module *m;
	t_test	 *t;
	int		  ret;
	int		  value;

	value = 42;
	ct_module_init(&m, "param", "parametrized tests");
	if (!m)
		return (EXIT_FAILURE);
	ret = param_guards(m, &value);
	if (ret != EXIT_SUCCESS)
		return (ct_module_destroy(m), ret);
	if (ct_module_taddf_param(m, param_expect, &value, "ok") != true)
		return (ct_module_destroy(m), 5);
	if (ct_module_taddf_param_timeout(m, param_expect, &value, "timed", 3) !=
		true)
		return (ct_module_destroy(m), 6);
	if (ct_module_taddf_param(m, param_fail, NULL, "ko") != true)
		return (ct_module_destroy(m), 7);
	if (ct_module_taddf(m, dummy_success_test, "pure") != true)
		return (ct_module_destroy(m), 8);
	ret = param_check_types(m, &value);
	if (ret != EXIT_SUCCESS)
		return (ct_module_destroy(m), 8 + ret);
	ct_module_run(m);
	if (m->tests_count != 4 || m->tests_passed != 3)
		return (ct_module_destroy(m), 16);
	t = find_test(m, "ko");
	if (!t || t->return_value == EXIT_SUCCESS)
		return (ct_module_destroy(m), 17);
	t = find_test(m, "ok");
	if (!t || t->return_value != EXIT_SUCCESS)
		return (ct_module_destroy(m), 18);
	return (ct_module_destroy(m), EXIT_SUCCESS);
}
