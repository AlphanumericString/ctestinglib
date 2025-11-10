/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tc_memory.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 15:42:18 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/01 15:42:18 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TC_MEMORY_H
# define TC_MEMORY_H

# include <stddef.h>
# include <stdbool.h>

# ifdef CTESTS_INTERNAL

void	push_to_cleanup(void *ptr);
void	do_cleanup(void (*f)(void *));

void	tci_set_memory_cleanup(bool val);
bool	tci_get_memory_cleanup(void);

# endif /* CTESTS_INTERNAL */

void	*safealloc(size_t size) __attribute__((malloc));
void	safefree(void *p);

void	tc_failloc_set(int nb);
size_t	tc_alloc_getcount(void);
void	tc_failloc_reset(void);

// launch f once and then re-launches it while reducing the number of
// allocations allowed before failure one by one until either a crash or 0
void	tc_auto_call(void (*f)(void));

#endif /* TC_MEMORY_H */
