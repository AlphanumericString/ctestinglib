#ifndef CT_UTILS_H
#define CT_UTILS_H

#include <stddef.h>
#ifdef CTESTS_INTERNAL
#include "ct_structs.h"

#define KO_COLOR "\033[31m"
#define OK_COLOR "\033[32m"
#define RESET_COLOR "\033[0m"

// cmp
int cti_cmp_alpha(const char *s1, const char *s2);

// lists
void  cti_list_push(t_list **lst, void *data);
void *cti_list_pop(t_list **lst);
void  cti_list_foreach(const t_list *list, void (*f)(void *data));

// printing utils
void cti_print_nb(int nb);
void cti_print_string(const char *str);
void cti_print_flt(float f);
void cti_print_charn(int depth, char c);

#endif /* CTESTS_INTERNAL */

void  ct_memset(void *ptr, int value, size_t size);
int	  ct_memcmp(const void *s1, const void *s2, size_t n);
void *ct_memcpy(void *dest, const void *src, size_t n);
void *ct_memmove(void *dest, const void *src, size_t n);
void *ct_memchr(const void *s, int c, size_t n);

int	   ct_strncmp(const char *s, const char *s2, size_t n);
size_t ct_strlen(const char *s);

#ifdef CTESTS_INTERNAL_TESTS

int ctit_memset(void);
int ctit_memcmp(void);
int ctit_memcpy(void);
int ctit_memmove(void);
int ctit_memchr(void);
int ctit_strncmp(void);
int ctit_strlen(void);

int ctit_listpush(void);
int ctit_listpop(void);
int ctit_listforeach(void);

int ctit_cmpalpha(void);

int ctit_print_cn(void);
int ctit_print_nb(void);
int ctit_print_flt(void);
int ctit_print_string(void);

#endif /* CTESTS_INTERNAL_TESTS */

#endif /* CT_UTILS_H */
