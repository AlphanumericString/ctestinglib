/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_module_add.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 08:40:51 by bgoulard          #+#    #+#             */
/*   Updated: 2025/10/28 08:40:51 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#define INTERNAL_TESTINGLIB
#define INTERNAL_TESTINGLIB_TESTS
#include "tests_fxtr.h"

int	tmodule_add(void)
{
	t_module	*test_root;

	test_root = safealloc(sizeof(t_module));
	if (!test_root)
		return (EXIT_FAILURE);
	init_module(test_root, "root", "root for tests");
	add_submodule(test_root, create_list_tests());
	if (test_root->module_count != 1 || test_root->submodules_list == NULL)
		return (module_destroy(test_root), 1);
	add_submodule(test_root, create_string_tests());
	if (test_root->module_count != 2 || test_root->submodules_list == NULL)
		return (module_destroy(test_root), 2);
	add_submodule(test_root, NULL);
	if (test_root->module_count != 2 || test_root->submodules_list == NULL)
		return (module_destroy(test_root), 3);
	return (module_destroy(test_root), EXIT_SUCCESS);
}
