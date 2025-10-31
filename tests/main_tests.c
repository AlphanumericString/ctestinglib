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
#include "tests_fxtr.h"
#include "memutils.h"

#define FSETUP	"Failed to setup tests"
#define FSETUP_SBM	"Failed to setup tests"

static void	__attribute__((unused)) display_diff_formats(t_module *th)
{
	write(STDOUT_FILENO, "=======default=\n", 16);
	display_results(th, DEFAULT);
	write(STDOUT_FILENO, "========detail=\n", 16);
	display_results(th, DETAIL_ALL);
	write(STDOUT_FILENO, "======dflt+sbm=\n", 16);
	display_results(th, DEFAULT | TM_PRINT_SBM);
	write(STDOUT_FILENO, "===ol+sbm+last=\n", 16);
	display_results(th, ONELINE | TM_PRINT_SBM | TM_PRINT_LAST);
	write(STDOUT_FILENO, "===========sbm=\n", 16);
	display_results(th, TM_PRINT_SBM);
	write(STDOUT_FILENO, "===========inf=\n", 16);
	display_results(th, TM_INF_ALL);
	write(STDOUT_FILENO, "=======oneline=\n", 16);
	display_results(th, ONELINE);
	write(STDOUT_FILENO, "========ol+sbm=\n", 16);
	display_results(th, ONELINE | TM_PRINT_SBM);
	write(STDOUT_FILENO, "========ol+lst=\n", 16);
	display_results(th, ONELINE | TM_PRINT_LAST);
	write(STDOUT_FILENO, "====ol+lst+sbm=\n", 16);
	display_results(th, ONELINE | TM_PRINT_LAST | TM_PRINT_SBM);
}

int	main(void)
{
	// t_module	*th;
	// int			ret;
	//
	// th = safealloc(sizeof(*th));
	// self_memset(th, 0, sizeof(*th));
	// init_module(th, "Main", "Dummy main to tests the project");
	// add_submodule(th, create_string_tests());
	// add_submodule(th, create_list_tests());
	// add_test_f(th, dummy_fail_test, "mf1");
	// add_test_f(th, dummy_success_test, "ms1");
	// run_module(th);
	// display_diff_formats(th);
	// ret = EXIT_SUCCESS;
	// if (th->tests_count != th->tests_passed
	// 	|| th->module_count != th->module_passed)
	// 	ret = EXIT_FAILURE;
	// return (module_destroy(th), ret);

	t_module	*root;
	t_module	*(*f[32])(void);
	int			ret;

	root = safealloc(sizeof(*root));
	if (!root)
		return (print_string(FSETUP), EXIT_FAILURE);
	init_module(root, "ctestinglib", "The library for which the tests are running");
	self_memset(f, 0, sizeof(f));
	if (!add_submodule(root, create_modules_tests())
		|| !add_submodule(root, create_tests_tests()))
		return (module_destroy(root), print_string(FSETUP_SBM), EXIT_FAILURE);
	ret = run_module(root);
	sort_recursive(root);
	display_results(root, DETAIL_ALL);
	return (module_destroy(root), ret);
}
