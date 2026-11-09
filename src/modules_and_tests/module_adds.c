#define CTESTS_INTERNAL
#include "ct_modules.h"
#include "ct_memory.h"
#include "ct_utils.h"

bool ct_module_add_sub(t_module *module, t_module *sub_module)
{
	if (!sub_module || !module)
		return (false);
	cti_list_push(&module->submodules_list, (void *)sub_module);
	sub_module->parent = (const t_module *)module;
	module->module_count++;
	return (true);
}

bool ct_module_taddf(t_module *m, int (*f)(void), const char *s)
{
	t_test *t;

	if (!m || !f || !s)
		return (false);
	t = safealloc(sizeof(*t));
	if (!t)
		return (false);
	*t = (t_test){0};
	t->return_value = 0;
	t->test_func = f;
	t->name = (char *)s;
	if (ct_module_tadd(m, t) == false)
		return (safefree(t), false);
	return (true);
}

bool ct_module_taddf_timeout(t_module *m, int (*f)(void), const char *s,
							 unsigned int seconds)
{
	if (ct_module_taddf(m, f, s) == false)
		return (false);
	((t_test *)m->tests_list->data)->timeout = seconds;
	return (true);
}

bool ct_module_tadd(t_module *module, const t_test *test)
{
	t_test *prev;

	if (!module || !test)
		return (false);
	prev = NULL;
	if (module->tests_list)
		prev = module->tests_list->data;
	cti_list_push(&module->tests_list, (void *)test);
	if (prev != test)
		module->tests_count++;
	return (prev != test);
}
