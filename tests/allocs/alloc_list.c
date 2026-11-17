#include <stdbool.h>
#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_internal.h"

static int	g_custom_free_calls;
static bool g_custom_free_untracked;
static bool g_auto_call_first_failed;
static int	g_auto_call_calls;

static void custom_free(void *p)
{
	g_custom_free_calls++;
	free(p);
}

static void custom_free_untracked(void *p)
{
	g_custom_free_calls++;
	g_custom_free_untracked = !is_in_cleanup(p);
	free(p);
}

static void auto_call_helper(void)
{
	void *a;
	void *b;

	g_auto_call_calls++;
	a = malloc(8);
	b = malloc(8);
	if (!a)
		g_auto_call_first_failed = true;
	if (a)
		free(a);
	if (b)
		free(b);
}

int ctit_cleanup(void)
{
	t_module *tmp_dummy;
	void	 *a;
	void	 *b;

	cti_set_memory_cleanup(true);
	a = malloc(32);
	b = malloc(32);
	if (!a || !b)
		return (cti_set_memory_cleanup(false), 1);
	do_cleanup(NULL);
	cti_set_memory_cleanup(true);
	a = malloc(32);
	if (!a)
		return (cti_set_memory_cleanup(false), 4);
	b = realloc(a, 4096);
	if (b != NULL)
		a = b;
	do_cleanup(NULL);
	cti_set_memory_cleanup(false);
	a = safealloc(32);
	b = safealloc(32);
	if (!a || !b)
		return (safefree(a), safefree(b), 2);
	push_to_cleanup(a);
	push_to_cleanup(b);
	g_custom_free_calls = 0;
	do_cleanup(custom_free);
	if (g_custom_free_calls != 2)
		return (3);
	do_cleanup(NULL);
	ct_module_init(&tmp_dummy, "module_dummy", "dummy module to test cleanups");
	ct_module_taddf(tmp_dummy, dummy_alloc, "alloc");
	tmp_dummy->auto_cleanup = true;
	ct_module_run(tmp_dummy);
	ct_module_destroy(tmp_dummy);
	return (EXIT_SUCCESS);
}

int ctit_count(void)
{
	size_t before;
	void  *p;

	before = ct_alloc_getcount();
	p = malloc(64);
	if (!p)
		return (1);
	if (ct_alloc_getcount() != before + 1)
		return (free(p), 2);
	free(p);
	if (cti_alloc_get() == NULL || cti_failloc_get() == NULL)
		return (3);
	return (EXIT_SUCCESS);
}

// The runner resets the allocation counter before every test, so a test starts
// from a known count whatever the setup hooks or the previous tests allocated.
int ctit_count_reset(void)
{
	void *p;

	if (ct_alloc_getcount() != 0)
		return (1);
	p = malloc(8);
	if (!p)
		return (2);
	if (ct_alloc_getcount() != 1)
		return (free(p), 3);
	free(p);
	return (EXIT_SUCCESS);
}

int ctit_fail(void)
{
	void *p;

	ct_failloc_reset();
	ct_failloc_set(0);
	if (malloc(16) != NULL)
		return (ct_failloc_reset(), 1);
	ct_failloc_set(1);
	p = malloc(16);
	if (!p)
		return (ct_failloc_reset(), 2);
	free(p);
	if (malloc(16) != NULL)
		return (ct_failloc_reset(), 3);
	ct_failloc_reset();
	p = malloc(16);
	if (p == NULL)
		return (4);
	free(p);
	g_auto_call_first_failed = false;
	g_auto_call_calls = 0;
	// A pre-set budget must not break the measurement run: with 2 allocations,
	// ct_auto_call() must call f() once to measure, then twice to fail each
	// allocation in turn. The previous budget must be restored afterwards.
	ct_failloc_set(1);
	ct_auto_call(auto_call_helper);
	if (g_auto_call_calls != 3)
		return (ct_failloc_reset(), 5);
	if (!g_auto_call_first_failed)
		return (ct_failloc_reset(), 6);
	if (*cti_failloc_get() != 1)
		return (ct_failloc_reset(), 7);
	if (ct_alloc_getcount() == 0)
		return (ct_failloc_reset(), 8);
	ct_failloc_reset();
	return (EXIT_SUCCESS);
}

int ctit_alloc_variants(void)
{
	size_t		   before;
	unsigned char *bytes;
	void		  *a;
	void		  *b;

	before = ct_alloc_getcount();
	a = calloc(4, 8);
	if (!a)
		return (1);
	bytes = a;
	if (ct_alloc_getcount() != before + 1)
		return (free(a), 2);
	if (bytes[0] != 0 || bytes[31] != 0)
		return (free(a), 3);
	b = realloc(a, 128);
	if (!b)
		return (free(a), 4);
	if (ct_alloc_getcount() != before + 2)
		return (free(b), 5);
	free(b);
	before = ct_alloc_getcount();
	a = realloc(NULL, 16);
	if (!a)
		return (6);
	if (ct_alloc_getcount() != before + 1)
		return (free(a), 7);
	free(a);
	ct_failloc_reset();
	ct_failloc_set(0);
	if (calloc(1, 8) != NULL)
		return (ct_failloc_reset(), 8);
	ct_failloc_reset();
	a = malloc(8);
	if (!a)
		return (9);
	ct_failloc_set(0);
	b = realloc(a, 16);
	if (b != NULL)
		return (ct_failloc_reset(), free(b), 10);
	ct_failloc_reset();
	b = realloc(a, 16);
	if (!b)
		return (free(a), 11);
	free(b);
	return (EXIT_SUCCESS);
}

int ctit_free(void)
{
	void *a;
	void *b;

	// free(NULL) stays a no-op while automatic cleanup is enabled.
	cti_set_memory_cleanup(true);
	free(NULL);

	// Freeing a tracked pointer only defers the release to do_cleanup(): the
	// pointer stays registered, so repeated free() calls can not double free.
	a = malloc(32);
	if (!a)
		return (cti_set_memory_cleanup(false), 1);
	free(a);
	if (!is_in_cleanup(a))
		return (cti_set_memory_cleanup(false), 2);
	free(a);
	if (!is_in_cleanup(a))
		return (cti_set_memory_cleanup(false), 3);
	do_cleanup(NULL);
	if (is_in_cleanup(a))
		return (4);

	// A pointer that is not registered (safealloc() memory) is released right
	// away, even while automatic cleanup is enabled.
	a = safealloc(32);
	if (!a)
		return (cti_set_memory_cleanup(false), 5);
	free(a);

	// A custom cleanup function may call free() itself: do_cleanup() unlinks
	// the node before the callback, so the pointer is not seen as tracked.
	a = malloc(32);
	b = malloc(32);
	if (!a || !b)
		return (cti_set_memory_cleanup(false), 6);
	free(a);
	g_custom_free_calls = 0;
	g_custom_free_untracked = false;
	do_cleanup(custom_free_untracked);
	if (g_custom_free_calls != 2 || g_custom_free_untracked == false)
		return (cti_set_memory_cleanup(false), 7);

	// Without automatic cleanup, free() behaves like the standard one.
	cti_set_memory_cleanup(false);
	a = malloc(32);
	if (!a)
		return (8);
	free(a);
	return (EXIT_SUCCESS);
}
