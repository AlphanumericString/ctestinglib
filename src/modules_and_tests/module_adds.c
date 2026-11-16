#define CTESTS_INTERNAL
#include "ct_internal.h"

/// @brief Attach a submodule to @p module.
/// @details The submodule becomes owned by @p module: it is destroyed with it
/// by ct_module_destroy().
/// @param module Parent module.
/// @param sub_module Submodule to attach.
/// @return true on success, false when either argument is NULL.
bool ct_module_add_sub(t_module *module, t_module *sub_module)
{
	if (!sub_module || !module)
		return (false);
	cti_list_push(&module->submodules_list, (void *)sub_module);
	sub_module->parent = (const t_module *)module;
	module->module_count++;
	return (true);
}

/// @brief Add a plain test to a module.
/// @details Allocates a test object that calls @p f and attaches it to @p m.
/// @param m Module to add the test to.
/// @param f Function to run; it returns EXIT_SUCCESS when the test passes.
/// @param s Name of the test, the string is not copied.
/// @return true on success, false when an argument is NULL or when the
/// allocation fails.
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
	t->type = CT_TPURE;
	t->test_func.pure = f;
	t->name = (char *)s;
	if (ct_module_tadd(m, t) == false)
		return (safefree(t), false);
	return (true);
}

/// @brief Add a plain test with a per-test timeout to a module.
/// @details Allocates a test object that calls @p f and attaches it to @p m,
/// with @p seconds as its timeout.
/// @param m Module to add the test to.
/// @param f Function to run; it returns EXIT_SUCCESS when the test passes.
/// @param s Name of the test, the string is not copied.
/// @param seconds Timeout in seconds; 0 falls back to the module timeout and
/// then to CT_DEFAULT_TIMEOUT.
/// @return true on success, false when an argument is NULL or when the
/// allocation fails.
bool ct_module_taddf_timeout(t_module *m, int (*f)(void), const char *s,
							 unsigned int seconds)
{
	if (ct_module_taddf(m, f, s) == false)
		return (false);
	((t_test *)m->tests_list->data)->timeout = seconds;
	return (true);
}

/// @brief Add a parametrized test to a module.
/// @details Allocates a test object and attaches it to @p m. When the test
/// runs it calls <tt>f(param)</tt>.
/// @param m Module to add the test to.
/// @param f Function to run with @p param.
/// @param param Pointer passed to @p f when the test runs, not copied.
/// @param s Name of the test, the string is not copied.
/// @return true on success, false when an argument is NULL or when the
/// allocation fails.
bool ct_module_taddf_param(t_module *m, int (*f)(void *), void *param,
						   const char *s)
{
	t_test *t;

	if (!m || !f || !s)
		return (false);
	t = safealloc(sizeof(*t));
	if (!t)
		return (false);
	*t = (t_test){0};
	t->return_value = 0;
	t->type = CT_PARAM;
	t->test_func.param = f;
	t->param = param;
	t->name = (char *)s;
	if (ct_module_tadd(m, t) == false)
		return (safefree(t), false);
	return (true);
}

/// @brief Add a parametrized test with a per-test timeout to a module.
/// @details Allocates a test object and attaches it to @p m. When the test
/// runs it calls <tt>f(param)</tt>, with @p seconds as its timeout.
/// @param m Module to add the test to.
/// @param f Function to run with @p param.
/// @param param Pointer passed to @p f when the test runs, not copied.
/// @param s Name of the test, the string is not copied.
/// @param seconds Timeout in seconds; 0 falls back to the module timeout and
/// then to CT_DEFAULT_TIMEOUT.
/// @return true on success, false when an argument is NULL or when the
/// allocation fails.
bool ct_module_taddf_param_timeout(t_module *m, int (*f)(void *), void *param,
								   const char *s, unsigned int seconds)
{
	if (ct_module_taddf_param(m, f, param, s) == false)
		return (false);
	((t_test *)m->tests_list->data)->timeout = seconds;
	return (true);
}

/// @brief Add an existing test object to a module.
/// @details The module takes ownership of @p test. Tests created by
/// ct_module_taddf() and its variants are normally attached through those
/// functions instead.
/// @param module Module to add the test to.
/// @param test Test object to attach.
/// @return true on success, false when either argument is NULL.
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
