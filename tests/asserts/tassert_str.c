#include "ct_capture.h"
#include <stdio.h>
#include <unistd.h>
#define CTESTS_INTERNAL_TESTS
#include "include/ct_assert.h"

int	ctit_streq(void)
{
	t_capture	*cap;

	cap = ct_capture_create(STDOUT_FILENO);
	ct_assert_str_eq("a", "b", "1\n");
	ct_assert_str_eq("a", "a", "2\n");
	ct_assert_str_eq("a", "b", NULL);
	ct_assert_str_eq("a", "a", NULL);
	if (ct_capture_compare(cap, "1\n") != 0)
		return (ct_capture_destroy(cap), 1);
	return (ct_capture_destroy(cap), 0);
}

int	ctit_strne(void)
{
	t_capture	*cap;

	cap = ct_capture_create(STDOUT_FILENO);
	ct_assert_str_neq("a", "b", "1\n");
	ct_assert_str_neq("a", "a", "2\n");
	ct_assert_str_neq("a", "b", NULL);
	ct_assert_str_neq("a", "a", NULL);
	if (ct_capture_compare(cap, "2\n") != 0)
		return (ct_capture_destroy(cap), 1);
	return (ct_capture_destroy(cap), 0);
}
