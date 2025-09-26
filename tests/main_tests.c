/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_tests.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:50:18 by bgoulard          #+#    #+#             */
/*   Updated: 2025/08/06 15:50:18 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

/* *********************** */
/*                         */
/* Functional tests main   */
/*                         */
/* Not compiled for lib    */
/*                         */
/* *********************** */

#define INTERNAL_TESTINGLIB
#define INTERNAL_TESTINGLIB_TESTS
#include "./include/tests_fxtr.h"
#include "./include/memutils.h"

static void	display_diff_formats(t_module *th)
{
	write(STDOUT_FILENO, "===============\n", 16);
	display_results(th, ONELINE);
	write(STDOUT_FILENO, "===============\n", 16);
	display_results(th, ONELINE | MODULE_PRINT_LAST);
	write(STDOUT_FILENO, "===============\n", 16);
	display_results(th, DEFAULT);
	write(STDOUT_FILENO, "===============\n", 16);
	display_results(th, DETAIL_ALL);
	write(STDOUT_FILENO, "===============\n", 16);
	display_results(th, DEFAULT | MODULE_PRINT_SBM);
	write(STDOUT_FILENO, "===============\n", 16);
	display_results(th, ONELINE | MODULE_PRINT_SBM | MODULE_PRINT_LAST);
}

int	main(void)
{
	t_module	*th;
	int			ret;

	th = safealloc(sizeof(*th));
	self_memset(th, 0, sizeof(*th));
	init_module(th, "main", "Main tests for the project");
	add_submodule(th, create_string_tests());
	add_submodule(th, create_list_tests());
	add_test_f(th, dummy_fail_test, "mf1");
	add_test_f(th, dummy_success_test, "ms1");
	run_module(th);
	display_diff_formats(th);
	ret = EXIT_SUCCESS;
	if (th->tests_count != th->tests_passed
		|| th->module_count != th->module_passed)
		ret = EXIT_FAILURE;
	return (module_destroy(th), ret);
}
