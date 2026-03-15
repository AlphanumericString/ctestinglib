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

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "tc_modules.h"

int	tcit_add(void)
{
	t_module	*test_root;

	tc_module_init(&test_root, "root", "root for tests");
	tc_module_add_sub(test_root, create_dummy_module());
	if (test_root->module_count != 1 || test_root->submodules_list == NULL)
		return (tc_module_destroy(test_root), 1);
	tc_module_add_sub(test_root, create_quiet_dummy());
	if (test_root->module_count != 2 || test_root->submodules_list == NULL)
		return (tc_module_destroy(test_root), 2);
	tc_module_add_sub(test_root, NULL);
	if (test_root->module_count != 2 || test_root->submodules_list == NULL)
		return (tc_module_destroy(test_root), 3);
	return (tc_module_destroy(test_root), EXIT_SUCCESS);
}
