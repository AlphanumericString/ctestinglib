#include <stdio.h>

#define CTESTS_INTERNAL
#include "ct_assert.h"

int ct_assert_true_v(bool condition, const char *err_msg, va_list args)
{
	if (!condition)
	{
		if (err_msg)
			vprintf(err_msg, args);
		return (1);
	}
	return (0);
}

int ct_assert_false_v(bool condition, const char *err_msg, va_list args)
{
	if (condition)
	{
		if (err_msg)
			vprintf(err_msg, args);
		return (1);
	}
	return (0);
}
