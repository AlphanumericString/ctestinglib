#define CTESTS_INTERNAL
#include "ct_memory.h"

// public version of cti_alloc_get, remoes the possibility of the user changing
// the values
size_t ct_alloc_getcount(void)
{
	return (*cti_alloc_get());
}

void ct_failloc_set(int nb)
{
	(*cti_failloc_get()) = (int64_t)nb;
}

void ct_failloc_reset(void)
{
	(*cti_failloc_get()) = INT64_MAX;
}

// This function is a wrapper around the standard malloc function.
// This is designed to tests the behavior of functions in case of memory
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
	if (cti_get_memory_cleanup() == true)
		push_to_cleanup(p);
	return (p);
}

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
