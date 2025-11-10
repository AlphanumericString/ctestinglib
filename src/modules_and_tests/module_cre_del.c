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

#define CTESTS_INTERNAL
#include "tc_modules.h"
#include "tc_memory.h"

static void	module_destroy_inner(t_module *m)
{
	const t_list	*sm_lst = m->submodules_list;
	const t_list	*ts_lst = m->tests_list;
	t_list			*tmp;

	while (sm_lst)
	{
		tc_module_destroy(sm_lst->data);
		tmp = (t_list *)sm_lst;
		sm_lst = sm_lst->next;
		safefree(tmp);
	}
	while (ts_lst)
	{
		tmp = (t_list *)ts_lst;
		ts_lst = ts_lst->next;
		safefree(tmp->data);
		safefree(tmp);
	}
	safefree(m);
	return ;
}

void	tc_module_destroy(t_module *m)
{
	if (!m)
		return ;
	module_destroy_inner(m);
}

void	tc_module_init(t_module **module, const char *name, const char *desc)
{
	t_module	*ret;

	if (!module || !name)
		return ;
	ret = safealloc(sizeof(t_module));
	if (!ret)
		return ;
	*ret = (t_module){0};
	ret->name = name;
	ret->description = desc;
	*module = ret;
}
