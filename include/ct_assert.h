#ifndef CT_ASSERT_H
#define CT_ASSERT_H

#include <stddef.h>
#include <stdbool.h>

#ifdef CTESTS_INTERNAL

#include <stdarg.h>

int ct_assert_false_v(bool condition, const char *err_msg, va_list args);
int ct_assert_true_v(bool condition, const char *err_msg, va_list args);

#endif /* CTESTS_INTERNAL */
#ifdef CTESTS_INTERNAL_TESTS

int ctit_true(void);
int ctit_false(void);
int ctit_streq(void);
int ctit_strne(void);

#endif /* CTESTS_INTERNAL_TESTS */

int ct_assert_true(bool condition, const char *err_msg, ...);
int ct_assert_false(bool condition, const char *err_msg, ...);

int ct_assert_str_eq(const char *s1, const char *s2, const char *err_msg, ...);
int ct_assert_str_neq(const char *s1, const char *s2, const char *err_msg, ...);

#endif /* CT_ASSERT_H */
