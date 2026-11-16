#ifndef CT_UTILS_H
#define CT_UTILS_H

#include <stddef.h>

#ifdef CTESTS_INTERNAL

#include "ct_structs.h"

#define KO_COLOR "\033[31m"
#define OK_COLOR "\033[32m"
#define RESET_COLOR "\033[0m"

#endif /* CTESTS_INTERNAL */

void  ct_memset(void *ptr, int value, size_t size);
int	  ct_memcmp(const void *s1, const void *s2, size_t n);
void *ct_memcpy(void *dest, const void *src, size_t n);
void *ct_memmove(void *dest, const void *src, size_t n);
void *ct_memchr(const void *s, int c, size_t n);

int	   ct_strncmp(const char *s, const char *s2, size_t n);
size_t ct_strlen(const char *s);

#endif /* CT_UTILS_H */
