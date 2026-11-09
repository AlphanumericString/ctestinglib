#define CTESTS_INTERNAL
#include "ct_assert.h"

int	ct_assert_true(bool condition, const char *err_msg, ...)
{
	va_list	args;
	int		ret;

	va_start(args, err_msg);
	ret = ct_assert_true_v(condition, err_msg, args);
	va_end(args);
	return (ret);
}

int	ct_assert_false(bool condition, const char *err_msg, ...)
{
	va_list	args;
	int		ret;

	va_start(args, err_msg);
	ret = ct_assert_false_v(condition, err_msg, args);
	va_end(args);
	return (ret);
}
