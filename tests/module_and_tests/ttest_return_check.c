#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_modules.h"

static int	specific_return_check(void)
{
	return (42);
}

int	ctit_return_check(void)
{
	const int	expected_returns[] = {42, EXIT_FAILURE, EXIT_SUCCESS};
	t_list		*it;
	t_module	*t;
	int			i;

	ct_module_init(&t, "dummy", "dummy for tests");
	ct_module_taddf(t, dummy_success_test, "success_test");
	ct_module_taddf(t, dummy_fail_test, "fail_test");
	ct_module_taddf(t, specific_return_check, "specific_return_test");
	i = ct_module_run(t);
	if (i != EXIT_SUCCESS || t->tests_count != 3 || t->tests_passed != 1)
		return (ct_module_destroy(t), 2);
	it = t->tests_list;
	while (it)
	{
		if (((t_test *)it->data)->return_value != expected_returns[i++])
			return (ct_module_destroy(t), 3 + i - 1);
		it = it->next;
	}
	return (ct_module_destroy(t), EXIT_SUCCESS);
}
