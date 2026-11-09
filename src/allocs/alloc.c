#define CTESTS_INTERNAL
#include "ct_memory.h"

extern void	*__libc_malloc(size_t size);
extern void	__libc_free(void *p);

// This is a by-pass around the malloc wrapper for the development of the
// testing library.
void	*safealloc(size_t size)
{
	return (__libc_malloc(size));
}

void	safefree(void *p)
{
	return (__libc_free(p));
}
