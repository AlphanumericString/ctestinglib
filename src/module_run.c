/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_run.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:18:13 by bgoulard          #+#    #+#             */
/*   Updated: 2025/08/17 01:36:07 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "tests_fxtr.h"
#include <stdlib.h>
#include <stdbool.h>

static int	run_module_inner(t_module *module)
{
	int			ret;
	t_list		*mod;

	mod = module->submodules_list;
	while (mod)
	{
		ret = run_module_inner(mod->data);
		if (ret == EXIT_SUCCESS)
			module->module_passed++;
		mod = mod->next;
	}
	return (run_test(module));
}

// static void	setup_pids(t_list *t_ls, int *pids, int **rets)
// {
// 	int	i;
//
// 	i = 0;
// 	while (t_ls)
// 	{
// 		pids[i] = ((t_test *)t_ls->data)->pid;
// 		rets[i] = &(((t_test *)t_ls->data)->return_value);
// 		t_ls = t_ls->next;
// 		i++;
// 	}
// }
//
// static int	check_pid(int *pid, int *ret, int *tests_passed)
// {
// 	int	w_resp;
//
// 	w_resp = waitpid(*pid, ret, WNOHANG);
// 	if (w_resp < 0) // error on waitpid
// 		return (EXIT_FAILURE);
// 	else if (w_resp == *pid) // pid finished + no error
// 	{
// 		*pid = 0;
// 		if (WIFEXITED(*ret) && WEXITSTATUS(*ret) == EXIT_SUCCESS)
// 			(*tests_passed)++;
// 	}
// 	else if (w_resp == 0) // pid not finished
// 		return (EXIT_SUCCESS);
// 	return (EXIT_SUCCESS);
// }
//
// static int	wait_pids(int *pids, int **rets, int tests_count, 
//		int *tests_passed)
// {
// 	int	i;
// 	bool	empty_pids;
//
// 	i = 0;
// 	empty_pids = false;
// 	while (!(i >= tests_count && empty_pids != false))
// 	{
// 		if (pids[i] > 0)
// 		{
// 			empty_pids = false;
// 			check_pid(&pids[i], rets[i], tests_passed);
// 		}
// 		i++;
// 		if (i >= tests_count && empty_pids == 0) {
// 			i = 0;
// 			empty_pids = true;;
// 		}
// 		usleep(100);
// 	}
// 	return (EXIT_SUCCESS);
// }
//
// static int	recoup_nohang(t_module *m, t_list *t_ls)
// {
// 	int		*pids;
// 	int		**rets;
//
// 	pids = safealloc(sizeof(int) * m->tests_count);
// 	rets = safealloc(sizeof(int *) * m->tests_count);
// 	setup_pids(t_ls, pids, rets);
// 	usleep(100 * 1000);
// 	wait_pids(pids, rets, m->tests_count, &m->tests_passed);
// 	return (free(pids), free(rets), EXIT_SUCCESS);
// }
//
//
// static int	recoup_return_values(t_module *module)
// {
// 	t_list	*m_ls;
//
// 	m_ls = module->submodules_list;
// 	while (m_ls)
// 	{
// 		if (recoup_return_values(m_ls->data) == EXIT_FAILURE)
// 			return (EXIT_FAILURE);
// 		m_ls = m_ls->next;
// 	}
// 	return (recoup_nohang(module, module->tests_list));
// }

// if (!module)
// 	return (EXIT_FAILURE);
// if (run_module_inner(module) == EXIT_FAILURE)
// 	return (EXIT_FAILURE);
// if (recoup_return_values(module) == EXIT_FAILURE)
// 	return (EXIT_FAILURE);

int	run_module(t_module *module)
{
	if (!module)
		return (EXIT_FAILURE);
	if (run_module_inner(module) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
/// TODO: for run_module
/// * Signal handler?
/// * modify t_test to keep pid and run the wait pid afterward
///		[bad, race conditions still apear in the setup shown above]
/// * Add a hook to run a setup/teardown for module/tests?
/// * Add a timeout for each test?
///		[can't do with fork&wait, might be possible to use usleep and kill]
