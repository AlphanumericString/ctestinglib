/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_cre_del.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:12:52 by bgoulard          #+#    #+#             */
/*   Updated: 2025/08/08 18:14:28 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define INTERNAL_TESTINGLIB
#include "tests_fxtr.h"
#include <stdlib.h>
// module destroy

static void	module_destroy_inner(t_module *m)
{
	const t_list	*sm_lst = m->submodules_list;
	const t_list	*ts_lst = m->tests_list;
	t_list			*tmp;

	while (sm_lst)
	{
		module_destroy(sm_lst->data);
		tmp = (t_list *)sm_lst;
		sm_lst = sm_lst->next;
		free(tmp);
	}
	while (ts_lst)
	{
		tmp = (t_list *)ts_lst;
		ts_lst = ts_lst->next;
		free(tmp->data);
		free(tmp);
	}
	free(m);
	return ;
}

void	module_destroy(t_module *m)
{
	if (!m)
		return ;
	module_destroy_inner(m);
}

void	init_module(t_module *module, const char *name, const char *desc)
{
	if (!module || !name)
		return ;
	*module = (t_module){0};
	module->name = name;
	module->description = desc;
}
