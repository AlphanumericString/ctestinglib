#include "ct_memory.h"
#include <stdio.h>
#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_modules.h"

int	ctit_init(void)
{
	const char	*deflt_names[] = {"Start name", "Module Name"};
	const char	*deflt_dscs[] = {"Start description", "Module description"};
	t_module	*mod_tests;
	t_module	*no_leak;

	mod_tests = NULL;
	ct_module_init(&mod_tests, NULL, deflt_dscs[1]);
	if (mod_tests != NULL)
		return (EXIT_FAILURE);
	ct_module_init(&mod_tests, deflt_names[1], NULL);
	if (mod_tests->name != deflt_names[1] || mod_tests->description != NULL)
		return (EXIT_FAILURE);
	mod_tests->submodules_list = (void *)0xDEADBEEF;
	mod_tests->tests_list = (void *)0xDEADCAFE;
	mod_tests->depth = 42;
	no_leak = mod_tests;
	ct_module_init(&mod_tests, deflt_names[0], deflt_dscs[0]);
	if (mod_tests->submodules_list != NULL || mod_tests->depth != 0
		|| mod_tests->tests_list != NULL || no_leak == mod_tests)
		return (EXIT_FAILURE);
	return (safefree(no_leak), ct_module_destroy(mod_tests), EXIT_SUCCESS);
}
