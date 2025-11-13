/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tc_utils.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 00:10:03 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/01 17:46:27 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TC_UTILS_H
# define TC_UTILS_H

# include <stddef.h>
# ifdef CTESTS_INTERNAL
#  include "tc_structs.h"

#  define KO_COLOR	"\033[31m"
#  define OK_COLOR	"\033[32m"
#  define RESET_COLOR	"\033[0m"

// cmp
int			tci_cmp_alpha(const char *s1, const char *s2);

// lists
void		tci_list_push(t_list **lst, void *data);
void		*tci_list_pop(t_list **lst);
void		tci_list_foreach(const t_list *list, void (*f)(void *data));

// printing utils
void		tci_print_nb(int nb);
void		tci_print_string(const char *str);
void		tci_print_flt(float f);
void		tci_print_charn(int depth, char c);

# endif /* CTESTS_INTERNAL */

void		tc_memset(void *ptr, int value, size_t size);
int			tc_memcmp(const void *s1, const void *s2, size_t n);
void		*tc_memcpy(void *dest, const void *src, size_t n);
void		*tc_memmove(void *dest, const void *src, size_t n);
void		*tc_memchr(const void *s, int c, size_t n);

int			tc_strncmp(const char *s, const char *s2, size_t n);
size_t		tc_strlen(const char *s);

# ifdef CTESTS_INTERNAL_TESTS

int			tcit_memset(void);
int			tcit_memcmp(void);
int			tcit_memcpy(void);
int			tcit_memmove(void);
int			tcit_memchr(void);
int			tcit_strncmp(void);
int			tcit_strlen(void);

int			tcit_listpush(void);
int			tcit_listpop(void);
int			tcit_listforeach(void);

int			tcit_cmpalpha(void);

int			tcit_print_cn(void);
int			tcit_print_nb(void);
int			tcit_print_flt(void);
int			tcit_print_string(void);

# endif /* CTESTS_INTERNAL_TESTS */

#endif /* TC_UTILS_H */
