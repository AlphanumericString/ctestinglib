/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_adds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:15:04 by bgoulard          #+#    #+#             */
/*   Updated: 2025/08/08 18:15:04 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define INTERNAL_TESTINGLIB
#include "tests_fxtr.h"
#include "memutils.h"

void	add_submodule(t_module *module, t_module *sub_module)
{
	list_push(&module->submodules_list, (void *)sub_module);
	sub_module->parent = (const t_module *)module;
	module->module_count++;
}

void	add_test_f(t_module *m, int (*f)(void), const char *s)
{
	t_test	*t;

	t = safealloc(sizeof(*t));
	self_memset(t, 0, sizeof(*t));
	t->return_value = 0;
	t->test_func = f;
	t->name = (char *)s;
	add_test(m, t);
}

void	add_test(t_module *module, const t_test *test)
{
	list_push(&module->tests_list, (void *)test);
	module->tests_count++;
}
