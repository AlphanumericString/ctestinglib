#include "ct_capture.h"
#include <unistd.h>
#define CTESTS_INTERNAL_TESTS
#include "include/ct_assert.h"

#define EXP_TRUE	"24"
#define EXP_FALSE	"13"
#define EXP_PRINT_ARG	"12 - blue - ff\n"

int	ctit_true(void)
{
	t_capture	*cap;
	int			ret;

	ret = 0;
	if (ct_assert_true(false, NULL) == 0 || ct_assert_true(true, NULL) != 0)
		return (1);
	cap = ct_capture_create(STDOUT_FILENO);
	ret += ct_assert_true(true, "1");
	ret += ct_assert_true(false, "2");
	ret += ct_assert_true(true, "%d", 3);
	ret += ct_assert_true(false, "%d", 4);
	ret += ct_assert_true(false, "%d - %s - %x\n", 12, "blue", 0xFF);
	if (ct_capture_compare(cap, EXP_TRUE EXP_PRINT_ARG) != 0 || ret != 3)
		return (ct_capture_destroy(cap), 1);
	return (ct_capture_destroy(cap), 0);
}

int	ctit_false(void)
{
	t_capture	*cap;
	int			ret;

	ret = 0;
	if (ct_assert_false(true, NULL) == 0 || ct_assert_false(false, NULL) != 0)
		return (1);
	cap = ct_capture_create(STDOUT_FILENO);
	ret += ct_assert_false(true, "1");
	ret += ct_assert_false(false, "2");
	ret += ct_assert_false(true, "%d", 3);
	ret += ct_assert_false(false, "%d", 4);
	ret += ct_assert_false(true, "%d - %s - %x\n", 12, "blue", 0xFF);
	if (ct_capture_compare(cap, EXP_FALSE EXP_PRINT_ARG) != 0 || ret != 3)
		return (ct_capture_destroy(cap), 1);
	return (ct_capture_destroy(cap), 0);
}
