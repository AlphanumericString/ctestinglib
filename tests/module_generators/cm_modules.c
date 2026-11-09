#define CTESTS_INTERNAL_TESTS
#include "ct_modules.h"

#define TCMODDESCS_PRE "Check on handling modules "
#define TCMODDESCS "functionalities"
#define TCMODDESCS_DISPLAY "display functionalities"
#define TCMODDESCS_RUN "running functionalities"
#define TCMODDESCS_HANDLING "handling(create/delete/addt/...) functionalities"

// taddf(module_display_tests, ctit_display_summary, "display summary");
//	...
//	...
static t_module	*create_modules_display_tests(void)
{
	t_module	*md_display;
	const char	*desc = TCMODDESCS_PRE TCMODDESCS_DISPLAY;

	ct_module_init(&md_display, "Modules Display", desc);
	if (!md_display)
		return (NULL);
	ct_module_taddf(md_display, ctit_module_display, "display module result");
	ct_module_taddf(md_display, ctit_module_display_extra,
		"display all formats");
	return (md_display);
}

static t_module	*create_modules_run_tests(void)
{
	t_module	*module_run_tests;
	const char	*desc = TCMODDESCS_PRE TCMODDESCS_RUN;

	ct_module_init(&module_run_tests, "Modules Run", desc);
	if (!module_run_tests)
		return (NULL);
	ct_module_taddf(module_run_tests, ctit_run, "running modules");
	ct_module_taddf(module_run_tests, ctit_module_hooks, "setup/teardown hooks");
	return (module_run_tests);
}

static t_module	*create_modules_handling_tests(void)
{
	t_module		*module_handling_tests;
	const char		*desc = TCMODDESCS_PRE TCMODDESCS_HANDLING;

	ct_module_init(&module_handling_tests, "Modules Handling", desc);
	if (!module_handling_tests)
		return (NULL);
	ct_module_taddf(module_handling_tests, ctit_add, "add submodules");
	ct_module_taddf(module_handling_tests, ctit_nb_tests, "nb tests");
	ct_module_taddf(module_handling_tests, ctit_nb_passed, "passed count");
	ct_module_taddf(module_handling_tests, ctit_ordering, "sorting");
	ct_module_taddf(module_handling_tests, ctit_init, "initializing modules");
	ct_module_taddf(module_handling_tests, ctit_module_errors,
		"error handling guards");
	return (module_handling_tests);
}

t_module	*create_modules_tests(void)
{
	t_module	*module_tests;
	t_module	*sub;
	t_module	*(*sub_creators[5])();
	size_t		i;

	ct_module_init(&module_tests, "Modules", TCMODDESCS);
	if (!module_tests)
		return (NULL);
	sub_creators[0] = create_modules_display_tests;
	sub_creators[1] = create_modules_run_tests;
	sub_creators[2] = create_modules_handling_tests;
	sub_creators[3] = NULL;
	i = 0;
	while (sub_creators[i])
	{
		sub = sub_creators[i++]();
		if (!sub || ct_module_add_sub(module_tests, sub) == false)
			return (ct_module_destroy(module_tests), NULL);
	}
	return (module_tests);
}

#undef TCMODDESCS
#undef TCMODDESCS_PRE
#undef TCMODDESCS_DISPLAY
#undef TCMODDESCS_RUN
#undef TCMODDESCS_HANDLING
