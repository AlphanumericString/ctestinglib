/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_modulesetups.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 08:51:28 by bgoulard          #+#    #+#             */
/*   Updated: 2025/10/28 08:51:28 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CTESTS_INTERNAL_TESTS
#include "tc_modules.h"

#define TCMODDESCS_PRE "Check on handling modules "
#define TCMODDESCS "functionalities"
#define TCMODDESCS_DISPLAY "display functionalities"
#define TCMODDESCS_RUN "running functionalities"
#define TCMODDESCS_HANDLING "handling(create/delete/addt/...) functionalities"


static t_module	*create_modules_display_tests(void)
{
	t_module	*module_display_tests;
	const char	*desc = TCMODDESCS_PRE TCMODDESCS_DISPLAY;

	tc_module_init(&module_display_tests, "Modules Display", desc);
	if (!module_display_tests)
		return (NULL);
//	tc_module_taddf(module_display_tests, tcit_display_summary, "display summary");
//	...
	return (module_display_tests);
}

static t_module	*create_modules_run_tests(void)
{
	t_module	*module_run_tests;
	const char	*desc = TCMODDESCS_PRE TCMODDESCS_RUN;

	tc_module_init(&module_run_tests, "Modules Run", desc);
	if (!module_run_tests)
		return (NULL);
	tc_module_taddf(module_run_tests, tcit_run, "running modules");
//	...
	return (module_run_tests);
}

static t_module	*create_modules_handling_tests(void)
{
	t_module		*module_handling_tests;
	const char		*desc = TCMODDESCS_PRE TCMODDESCS_HANDLING;

	tc_module_init(&module_handling_tests, "Modules Handling", desc);
	if (!module_handling_tests)
		return (NULL);
	tc_module_taddf(module_handling_tests, tcit_add, "add submodules");
	tc_module_taddf(module_handling_tests, tcit_nb_tests, "nb tests");
	tc_module_taddf(module_handling_tests, tcit_nb_passed, "passed count");
	tc_module_taddf(module_handling_tests, tcit_ordering, "sorting");
	tc_module_taddf(module_handling_tests, tcit_init, "initializing modules");
	return (module_handling_tests);
}

t_module	*create_modules_tests(void)
{
	t_module	*module_tests;
	t_module	*sub;
	t_module	*(*sub_creators[5])();
	size_t		i;

	tc_module_init(&module_tests, "Modules", TCMODDESCS);
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
		if (!sub || tc_module_add_sub(module_tests, sub) == false)
			return (tc_module_destroy(module_tests), NULL);
	}
	return (module_tests);
}

#undef TCMODDESCS
#undef TCMODDESCS_PRE
#undef TCMODDESCS_DISPLAY
#undef TCMODDESCS_RUN
#undef TCMODDESCS_HANDLING
