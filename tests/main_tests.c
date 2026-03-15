/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_tests.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:50:18 by bgoulard          #+#    #+#             */
/*   Updated: 2026/01/22 12:59:32 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tc_structs.h"
#include <stdlib.h>

/* *********************** */
/*                         */
/* Functional tests main   */
/*                         */
/* Not compiled for lib    */
/*                         */
/* *********************** */

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "tc_modules.h"
#include "tc_utils.h"

#define FSETUP	"Failed to setup tests"
#define FSET_SBM	"Failed to setup tests"
#define RDESC	"The library for which the tests are running"

// static void	__attribute__((unused))	display_diff_formats(t_module *th)
// {
// 	write(STDOUT_FILENO, "=======default=\n", 16);
// 	tc_module_display(th, DEFAULT);
// 	write(STDOUT_FILENO, "========detail=\n", 16);
// 	tc_module_display(th, DETAIL_ALL);
// 	write(STDOUT_FILENO, "======dflt+sbm=\n", 16);
// 	tc_module_display(th, DEFAULT | TM_PRINT_SBM);
// 	write(STDOUT_FILENO, "===ol+sbm+last=\n", 16);
// 	tc_module_display(th, ONELINE | TM_PRINT_SBM | TM_PRINT_LAST);
// 	write(STDOUT_FILENO, "===========sbm=\n", 16);
// 	tc_module_display(th, TM_PRINT_SBM);
// 	write(STDOUT_FILENO, "===========inf=\n", 16);
// 	tc_module_display(th, TM_INF_ALL);
// 	write(STDOUT_FILENO, "=======oneline=\n", 16);
// 	tc_module_display(th, ONELINE);
// 	write(STDOUT_FILENO, "========ol+sbm=\n", 16);
// 	tc_module_display(th, ONELINE | TM_PRINT_SBM);
// 	write(STDOUT_FILENO, "========ol+lst=\n", 16);
// 	tc_module_display(th, ONELINE | TM_PRINT_LAST);
// 	write(STDOUT_FILENO, "====ol+lst+sbm=\n", 16);
// 	tc_module_display(th, ONELINE | TM_PRINT_LAST | TM_PRINT_SBM);
// }

static bool	add_modules(t_module *root, t_module *(**f)(void))
{
	size_t	i;

	if (!f || !root)
		return (false);
	i = 0;
	while (f[i])
		if (!tc_module_add_sub(root, f[i++]()))
			return (false);
	return (true);
}

int	main(void)
{
	t_module	*root;
	int			ret;

	tc_module_init(&root, "ctestinglib", RDESC);
	if (!add_modules(root, (t_module *(*[])(void)){create_modules_tests,
		create_tests_tests, create_utils_tests, NULL}))
		return (tc_module_destroy(root), tci_print_string(FSET_SBM),
			EXIT_FAILURE);
	ret = tc_module_run(root);
	tc_module_sort(root);
	tc_module_display(root, 0);
	return (tc_module_destroy(root), ret);
}
