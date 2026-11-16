#define CTESTS_INTERNAL
#include "ct_internal.h"

extern void *__libc_malloc(size_t size);
extern void	 __libc_free(void *p);

/// @brief Allocate memory, bypassing the library's allocation wrapper.
/// @details Unlike malloc(), this function is not counted by
/// ct_alloc_getcount() and is not affected by ct_failloc_set(). It is used by
/// the library itself for its own bookkeeping so that the library never
/// perturbs the allocation tests of the code under test.
/// @param size Number of bytes to allocate.
/// @return Pointer to the allocated memory, or NULL on failure.
/// @note The returned memory must be released with safefree().
void *safealloc(size_t size)
{
	return (__libc_malloc(size));
}

/// @brief Free memory allocated with safealloc().
/// @param p Pointer returned by safealloc(), NULL is accepted.
void safefree(void *p)
{
	return (__libc_free(p));
}
