#ifndef CT_MEMORY_H
#define CT_MEMORY_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * @file ct_memory.h
 * @brief Allocation helpers and allocation failure injection.
 *
 * The library wraps malloc(), calloc() and realloc() to count allocations and
 * to make a chosen allocation fail. free() is wrapped as well: when automatic
 * cleanup is enabled, freeing a pointer that is registered for cleanup is
 * deferred to the cleanup pass, so calling free() on it repeatedly, or letting
 * the cleanup run after it, can not double free it.
 * safealloc() and safefree() bypass those wrappers and are used by the library
 * itself for its own bookkeeping. safefree() is also the escape hatch for a
 * custom cleanup function that has to release a pointer itself.
 * Duplicating helpers such as strdup() and memdup() are intentionally not
 * wrapped: they are not direct allocation-failure exploration entry points.
 *
 * ct_auto_call() exercises a function with each of its allocations failing in
 * turn, including the first one, which is a lightweight way to check that it
 * handles allocation failures.
 *
 * The allocation counter read by ct_alloc_getcount() is reset by the test
 * runner before each test, so it measures the allocations of the current test.
 */

void *safealloc(size_t size) __attribute__((malloc));
void  safefree(void *p);

void   ct_failloc_set(int nb);
size_t ct_alloc_getcount(void);
void   ct_failloc_reset(void);

void ct_auto_call(void (*f)(void));

#endif /* CT_MEMORY_H */
