#include <stdbool.h>
#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_internal.h"

static int g_custom_free_calls;

static void custom_free(void *p)
{
	g_custom_free_calls++;
	free(p);
}

static void auto_call_helper(void)
{
	void *a;
	void *b;

	a = malloc(8);
	b = malloc(8);
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
	ct_auto_call(auto_call_helper);
	ct_failloc_reset();
	if (ct_alloc_getcount() == 0)
		return (5);
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
