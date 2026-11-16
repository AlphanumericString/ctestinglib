#include <string.h>

#define CTESTS_INTERNAL
#include "ct_internal.h"

/// @brief Check that two strings are equal.
/// @details The comparison is done with strcmp(), so both strings must be
/// valid and NUL terminated.
/// @param s1 First string.
/// @param s2 Second string.
/// @param err_msg printf-like format string printed on failure, or NULL to
/// print nothing.
/// @param ... Optional arguments for @p err_msg.
/// @return 0 when the strings are equal, 1 otherwise.
int ct_assert_str_eq(const char *s1, const char *s2, const char *err_msg, ...)
{
	va_list args;
	int		ret;

	va_start(args, err_msg);
	ret = ct_assert_true_v(strcmp(s1, s2) == 0, err_msg, args);
	va_end(args);
	return (ret);
}

/// @brief Check that two strings are different.
/// @details The comparison is done with strcmp(), so both strings must be
/// valid and NUL terminated.
/// @param s1 First string.
/// @param s2 Second string.
/// @param err_msg printf-like format string printed on failure, or NULL to
/// print nothing.
/// @param ... Optional arguments for @p err_msg.
/// @return 0 when the strings differ, 1 otherwise.
int ct_assert_str_neq(const char *s1, const char *s2, const char *err_msg, ...)
{
	va_list args;
	int		ret;

	va_start(args, err_msg);
	ret = ct_assert_true_v(strcmp(s1, s2) != 0, err_msg, args);
	va_end(args);
	return (ret);
}
