#ifndef CT_ASSERT_H
#define CT_ASSERT_H

#include <stddef.h>
#include <stdbool.h>

int ct_assert_true(bool condition, const char *err_msg, ...);
int ct_assert_false(bool condition, const char *err_msg, ...);

int ct_assert_str_eq(const char *s1, const char *s2, const char *err_msg, ...);
int ct_assert_str_neq(const char *s1, const char *s2, const char *err_msg, ...);

#endif /* CT_ASSERT_H */
