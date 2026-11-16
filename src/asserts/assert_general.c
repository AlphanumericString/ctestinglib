#define CTESTS_INTERNAL
#include "ct_internal.h"

/// @brief Check that @p condition is true.
/// @details When @p condition is false, @p err_msg is printed on stdout with
/// vprintf() and the assertion fails.
/// @param condition Value expected to be true.
/// @param err_msg printf-like format string printed on failure, or NULL to
/// print nothing.
/// @param ... Optional arguments for @p err_msg.
/// @return 0 when the assertion passes, 1 otherwise.
int ct_assert_true(bool condition, const char *err_msg, ...)
{
	va_list args;
	int		ret;

	va_start(args, err_msg);
	ret = ct_assert_true_v(condition, err_msg, args);
	va_end(args);
	return (ret);
}

/// @brief Check that @p condition is false.
/// @details When @p condition is true, @p err_msg is printed on stdout with
/// vprintf() and the assertion fails.
/// @param condition Value expected to be false.
/// @param err_msg printf-like format string printed on failure, or NULL to
/// print nothing.
/// @param ... Optional arguments for @p err_msg.
/// @return 0 when the assertion passes, 1 otherwise.
int ct_assert_false(bool condition, const char *err_msg, ...)
{
	va_list args;
	int		ret;

	va_start(args, err_msg);
	ret = ct_assert_false_v(condition, err_msg, args);
	va_end(args);
	return (ret);
}
