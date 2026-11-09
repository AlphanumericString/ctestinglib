/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tc_memory.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 15:42:18 by bgoulard          #+#    #+#             */
/*   Updated: 2026/01/22 13:37:35 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TC_MEMORY_H
# define TC_MEMORY_H

# include <stddef.h>
# include <stdbool.h>
# include <stdint.h>

# ifdef CTESTS_INTERNAL

int64_t	*tci_failloc_get(void);
size_t	*tci_alloc_get(void);

void	push_to_cleanup(void *ptr);
void	do_cleanup(void (*f)(void *));

void	tci_set_memory_cleanup(bool val);
bool	tci_get_memory_cleanup(void);

# endif /* CTESTS_INTERNAL */
# ifdef CTESTS_INTERNAL_TESTS

int tcit_cleanup(void);
int tcit_count(void);
int tcit_fail(void);

# endif /* CTESTS_INTERNAL_TESTS */

void	*safealloc(size_t size) __attribute__((malloc));
void	safefree(void *p);

void	tc_failloc_set(int nb);
size_t	tc_alloc_getcount(void);
void	tc_failloc_reset(void);

/// @brief	essitmates the number of calls to malloc and then call the function
/// f with a fail point decreasing until either a crash or 0.
/// view this as a light funcheck.
/// @param f	the function to call
void	tc_auto_call(void (*f)(void));

#endif /* TC_MEMORY_H */
