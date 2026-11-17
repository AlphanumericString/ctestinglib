#include <stdlib.h>

#define CTESTS_INTERNAL
#include "ct_internal.h"

/// @brief Return the number of allocations that have succeeded so far.
/// @details The counter is incremented by every call to malloc(), calloc() or
/// realloc() that goes through the library wrappers; safealloc() does not
/// increment it.
/// @return Number of successful allocations since the program started.
size_t ct_alloc_getcount(void)
{
	return (*cti_alloc_get());
}

/// @brief Set how many allocations may succeed before the next one fails.
/// @details The next @p nb calls to malloc(), calloc() or realloc() succeed and
/// the allocation after them returns NULL. A value of 0 or less makes the next
/// allocation fail immediately.
/// @param nb Number of allocations allowed to succeed.
/// @note ct_failloc_reset() disables the failure injection.
void ct_failloc_set(int nb)
{
	(*cti_failloc_get()) = (int64_t)nb;
}

/// @brief Disable allocation failure injection.
/// @details Resets the failure budget so that allocations always return memory
/// again. This is the state the library restores before running each test.
void ct_failloc_reset(void)
{
	(*cti_failloc_get()) = INT64_MAX;
}

// These functions are wrappers around the standard allocation functions.
// They are designed to test the behavior of functions in case of memory
// allocation failure.
// DO NOT USE TO DEV THE TESTING LIB!!!
void *malloc(size_t size)
{
	void *p;

	if (*cti_failloc_get() <= 0)
		return (NULL);
	(*cti_alloc_get())++;
	(*cti_failloc_get())--;
	p = safealloc(size);
	if (p != NULL && cti_get_memory_cleanup() == true)
		push_to_cleanup(p);
	return (p);
}

void *calloc(size_t nmemb, size_t size)
{
	void *p;

	if (*cti_failloc_get() <= 0)
		return (NULL);
	(*cti_alloc_get())++;
	(*cti_failloc_get())--;
	p = safecalloc(nmemb, size);
	if (p != NULL && cti_get_memory_cleanup() == true)
		push_to_cleanup(p);
	return (p);
}

void *realloc(void *ptr, size_t size)
{
	void *p;
	bool  tracked;

	if (*cti_failloc_get() <= 0)
		return (NULL);
	(*cti_alloc_get())++;
	(*cti_failloc_get())--;
	p = saferealloc(ptr, size);
	if (cti_get_memory_cleanup() == true)
	{
		if (p != NULL)
		{
			tracked = (ptr == NULL) || remove_from_cleanup(ptr);
			if (tracked)
				push_to_cleanup(p);
		}
		else if (size == 0)
			remove_from_cleanup(ptr);
	}
	return (p);
}

/// @brief Exercise a function with each of its allocations failing in turn.
/// @details The function is first called once to measure how many allocations
/// it performs, then called again with a decreasing allocation budget so that
/// each of its allocations, except the first, is made to fail in turn. This is
/// a lightweight, funcheck-like way to check that a function handles
/// allocation failures without crashing.
/// @param f Function to call; it takes no argument and should perform the same
/// allocations on every run.
void ct_auto_call(void (*f)(void))
{
	const size_t current_ac = ct_alloc_getcount();
	size_t		 after_count;

	f();
	after_count = ct_alloc_getcount();
	while (after_count != current_ac)
	{
		ct_failloc_set(after_count - current_ac);
		f();
		after_count--;
	}
}
