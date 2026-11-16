#ifndef CT_ASSERT_H
#define CT_ASSERT_H

#include <stddef.h>
#include <stdbool.h>

/**
 * @file ct_assert.h
 * @brief Assertions for test functions.
 *
 * Every assertion returns 0 on success and 1 on failure, so a test function
 * can simply return the result of the assertion it wants to check:
 *
 * @code
 * int my_test(void)
 * {
 *     return (ct_assert_true(1 + 1 == 2, "1 + 1 should equal 2\n"));
 * }
 * @endcode
 *
 * @c err_msg is a printf-like format string (it may be NULL to print nothing)
 * whose optional arguments are printed on stdout when the assertion fails.
 */

int ct_assert_true(bool condition, const char *err_msg, ...);
int ct_assert_false(bool condition, const char *err_msg, ...);

int ct_assert_str_eq(const char *s1, const char *s2, const char *err_msg, ...);
int ct_assert_str_neq(const char *s1, const char *s2, const char *err_msg, ...);

#endif /* CT_ASSERT_H */
