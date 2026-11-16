#include <string.h>

#define CTESTS_INTERNAL
#include "ct_internal.h"

int ct_assert_str_eq(const char *s1, const char *s2, const char *err_msg, ...)
{
	va_list args;
	int		ret;

	va_start(args, err_msg);
	ret = ct_assert_true_v(strcmp(s1, s2) == 0, err_msg, args);
	va_end(args);
	return (ret);
}

int ct_assert_str_neq(const char *s1, const char *s2, const char *err_msg, ...)
{
	va_list args;
	int		ret;

	va_start(args, err_msg);
	ret = ct_assert_true_v(strcmp(s1, s2) != 0, err_msg, args);
	va_end(args);
	return (ret);
}
