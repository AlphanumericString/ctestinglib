#include <stdbool.h>
#include <stdlib.h>

#define CTESTS_INTERNAL
#include "ct_internal.h"

static void enable_cleanup(t_module *m)
{
	if (m->auto_cleanup == true)
		cti_set_memory_cleanup(true);
}

static void disable_cleanup(t_module *m)
{
	if (m->auto_cleanup == true)
	{
		do_cleanup(m->clean_up_function);
		cti_set_memory_cleanup(false);
	}
}

/// @brief Prepare a module's fixture.
/// @details Enables automatic cleanup when requested, then calls the setup
/// hook. The teardown hook is expected to run even when setup failed, so that
/// a partially initialised fixture can be released.
/// @param m Module to set up.
/// @return EXIT_FAILURE when the setup hook reported a failure.
int cti_module_setup(t_module *m)
{
	if (!m)
		return (EXIT_FAILURE);
	enable_cleanup(m);
	if (m->setup_function && m->setup_function() == false)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/// @brief Release a module's fixture.
/// @details Releases the automatic cleanup allocations when the module enabled
/// it, then calls the teardown hook.
/// @param m Module to tear down.
/// @return EXIT_FAILURE when the teardown hook reported a failure.
int cti_module_teardown(t_module *m)
{
	int ret;

	if (!m)
		return (EXIT_FAILURE);
	ret = EXIT_SUCCESS;
	disable_cleanup(m);
	if (m->teardown_function && m->teardown_function() == false)
		ret = EXIT_FAILURE;
	return (ret);
}

/// @brief Run only the tests directly attached to @p module.
/// @details No hook is called and no cleanup is performed: this is the test
/// loop used once the fixture is already set up.
/// @param module Module whose tests are run.
/// @return EXIT_SUCCESS when every test passed, EXIT_FAILURE otherwise.
int cti_module_run_tests(t_module *module)
{
	t_list *t_ls;
	int		ret;

	if (!module)
		return (EXIT_FAILURE);
	ret = EXIT_SUCCESS;
	t_ls = module->tests_list;
	while (t_ls)
	{
		ct_failloc_reset();
		cti_alloc_reset();
		if (cti_tfork(t_ls->data, module) != EXIT_SUCCESS)
			ret = EXIT_FAILURE;
		t_ls = t_ls->next;
	}
	return (ret);
}

/// @brief Run only the tests directly attached to @p module.
/// @details The module setup hook is called before the tests and its teardown
/// hook after them. When auto cleanup is enabled, every allocation made by the
/// tests is released after the last test. A failing setup skips the tests but
/// the teardown hook still runs; a failing setup or teardown makes the module
/// fail.
/// @param module Module whose tests are run.
/// @return EXIT_SUCCESS when every test passed, EXIT_FAILURE otherwise.
int ct_module_run_self_tests(t_module *module)
{
	int ret;

	if (!module)
		return (EXIT_FAILURE);
	ret = cti_module_setup(module);
	if (ret == EXIT_SUCCESS)
		ret = cti_module_run_tests(module);
	if (cti_module_teardown(module) != EXIT_SUCCESS)
		ret = EXIT_FAILURE;
	return (ret);
}
