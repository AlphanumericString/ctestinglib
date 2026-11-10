#include <stdio.h>
#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_modules.h"

static int leaf_ordering_check(t_module *t)
{
	const void *expected_forder[] = {dummy_success_test, (void *)0xDEAF,
									 (void *)0xCAFE, dummy_fail_test,
									 (void *)0xDEAD};
	t_list	   *leaf;
	size_t		i;

	i = 0;
	leaf = t->tests_list;
	while (leaf)
	{
		if ((void *)((t_test *)leaf->data)->test_func != expected_forder[i++])
			return (EXIT_FAILURE + i - 1);
		leaf = leaf->next;
	}
	return (i - t->tests_count);
}

static t_module *local_dummy_module_create(void)
{
	t_module *t;

	ct_module_init(&t, "dummy", "module created for testing purposes");
	ct_module_taddf(t, dummy_success_test, "aaa");
	ct_module_taddf(t, dummy_fail_test, "aba");
	ct_module_taddf(t, (void *)0xDEAD, "baa");
	ct_module_taddf(t, (void *)0xCAFE, "aab");
	ct_module_taddf(t, (void *)0xDEAF, "aaaa");
	return (t);
}

int ctit_tordering(void)
{
	t_module *t;
	t_module *sub;

	t = local_dummy_module_create();
	if (!t)
		return (EXIT_FAILURE);
	ct_module_sort(t);
	if (leaf_ordering_check(t))
		return (ct_module_destroy(t), 1);
	sub = create_quiet_dummy();
	if (!sub)
		return (ct_module_destroy(t), EXIT_FAILURE);
	ct_module_add_sub(t, sub);
	ct_module_sort(t);
	if (leaf_ordering_check(sub))
		return (ct_module_destroy(t), 2);
	return (ct_module_destroy(t), EXIT_SUCCESS);
}
