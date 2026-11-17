#include <stdlib.h>

#define CTESTS_INTERNAL
#include "ct_internal.h"

/// @brief Return the number of allocations that have succeeded so far.
/// @details The counter is incremented by every call to malloc(), calloc() or
/// realloc() that goes through the library wrappers; safealloc() does not
/// increment it. The test runner resets the counter before each test, so it
/// measures the allocations of the current test.
/// @return Number of successful allocations since the counter was last reset.
size_t ct_alloc_getcount(void)
{
	return (*cti_alloc_get());
}

/// @brief Reset the allocation counter to zero.
/// @details The counter reported by ct_alloc_getcount() is process-wide, so
/// allocations made outside of the code under test shift it. The test runner
/// calls this before each test so that every test starts from a known count.
void cti_alloc_reset(void)
{
	(*cti_alloc_get()) = 0;
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

// free() wrapper: while automatic memory cleanup is enabled, pointers that are
// registered for cleanup are left untouched so that the cleanup pass remains
// their only owner. This makes a mid-test free() harmless: calling it several
// times, or letting do_cleanup() run afterwards, can not double free anything.
// Pointers outside the cleanup list (safealloc() memory, cleanup disabled, ...)
// are released immediately, so the allocator still reports genuine double
// frees of untracked pointers.
// DO NOT USE TO DEV THE TESTING LIB!!!
void free(void *ptr)
{
	if (ptr == NULL)
		return;
	if (cti_get_memory_cleanup() == true)
	{
		if (is_in_cleanup(ptr) == true)
			return;
	}
	else
		remove_from_cleanup(ptr);
	safefree(ptr);
}

/// @brief Exercise a function with each of its allocations failing in turn.
/// @details The function is first called once to measure how many allocations
/// it performs, then called again with a decreasing allocation budget so that
/// each of its allocations is made to fail in turn, starting with the last one
/// and ending with the first: a budget of 0 makes the very first allocation
/// fail. This is a lightweight, funcheck-like way to check that a function
/// handles allocation failures without crashing.
/// @param f Function to call; it takes no argument and should perform the same
/// allocations on every run.
/// @note Failure injection is disabled while measuring the allocations, and the
/// previous failure budget is restored when the function returns.
void ct_auto_call(void (*f)(void))
{
	const int64_t prev_failloc = (*cti_failloc_get());
	const size_t  current_ac = ct_alloc_getcount();
	size_t		  nb_allocs;

	ct_failloc_reset();
	f();
	nb_allocs = ct_alloc_getcount() - current_ac;
	while (nb_allocs > 0)
	{
		nb_allocs--;
		ct_failloc_set((int)nb_allocs);
		f();
	}
	(*cti_failloc_get()) = prev_failloc;
}
