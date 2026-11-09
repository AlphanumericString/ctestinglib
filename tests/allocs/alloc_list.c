#include <stdbool.h>
#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_memory.h"
#include "ct_modules.h"
#include "ct_utils.h"

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
