#ifndef CT_MEMORY_H
# define CT_MEMORY_H

# include <stddef.h>
# include <stdbool.h>
# include <stdint.h>

# ifdef CTESTS_INTERNAL

int64_t	*cti_failloc_get(void);
size_t	*cti_alloc_get(void);

void	push_to_cleanup(void *ptr);
void	do_cleanup(void (*f)(void *));

void	cti_set_memory_cleanup(bool val);
bool	cti_get_memory_cleanup(void);

# endif /* CTESTS_INTERNAL */
# ifdef CTESTS_INTERNAL_TESTS

int ctit_cleanup(void);
int ctit_count(void);
int ctit_fail(void);

# endif /* CTESTS_INTERNAL_TESTS */

void	*safealloc(size_t size) __attribute__((malloc));
void	safefree(void *p);

void	ct_failloc_set(int nb);
size_t	ct_alloc_getcount(void);
void	ct_failloc_reset(void);

/// @brief	essitmates the number of calls to malloc and then call the function
/// f with a fail point decreasing until either a crash or 0.
/// view this as a light funcheck.
/// @param f	the function to call
void	ct_auto_call(void (*f)(void));

#endif /* CT_MEMORY_H */
