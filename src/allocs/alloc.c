#define CTESTS_INTERNAL
#include "ct_internal.h"

extern void *__libc_malloc(size_t size);
extern void *__libc_calloc(size_t nmemb, size_t size);
extern void *__libc_realloc(void *ptr, size_t size);
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

/// @brief Allocate zero-initialised memory, bypassing the allocation wrapper.
/// @details Unlike calloc(), this function is not counted by
/// ct_alloc_getcount() and is not affected by ct_failloc_set().
/// @param nmemb Number of elements to allocate.
/// @param size Size of each element.
/// @return Pointer to the zero-initialised memory, or NULL on failure.
/// @note The returned memory must be released with safefree().
void *safecalloc(size_t nmemb, size_t size)
{
	return (__libc_calloc(nmemb, size));
}

/// @brief Resize memory, bypassing the library's allocation wrapper.
/// @details Unlike realloc(), this function is not counted by
/// ct_alloc_getcount() and is not affected by ct_failloc_set().
/// @param ptr Pointer to resize, NULL is accepted.
/// @param size New size in bytes.
/// @return Pointer to the resized memory, or NULL on failure.
/// @note The returned memory must be released with safefree().
void *saferealloc(void *ptr, size_t size)
{
	return (__libc_realloc(ptr, size));
}

/// @brief Free memory allocated with safealloc().
/// @param p Pointer returned by safealloc(), NULL is accepted.
void safefree(void *p)
{
	return (__libc_free(p));
}
