/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmodule_display_extra.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 15:30:00 by bgoulard          #+#    #+#             */
/*   Updated: 2026/01/22 15:30:00 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "tc_modules.h"

static int	display_empty(void)
{
	t_module	*empty;

	tc_module_init(&empty, "empty", "no tests");
	tc_module_display(empty, TC_DM_SUMPCT);
	tc_module_display(empty, TC_DM_SUMPCT | TC_FMT_JSN);
	tc_module_display(empty, TC_DM_SUMNBS);
	tc_module_display(empty, TC_DISPLAY_DEFAULT);
	tc_module_destroy(empty);
	return (EXIT_SUCCESS);
}

static int	display_results(void)
{
	t_module	*fail;

	tc_module_init(&fail, "fail", "failing results");
	tc_module_taddf(fail, dummy_fail_test, "ko");
	tc_module_taddf(fail, dummy_sigsegv, "crash");
	tc_module_run(fail);
	tc_module_display(fail, TC_DT_OK);
	tc_module_display(fail, TC_FMT_JSN);
	tc_module_display(fail, TC_FMT_JSN | TC_DT_OK);
	tc_module_destroy(fail);
	return (EXIT_SUCCESS);
}

static int	display_submodules(void)
{
	t_module	*root;
	t_module	*sub1;
	t_module	*sub2;

	tc_module_init(&root, "root", "root description");
	sub1 = create_quiet_dummy();
	sub2 = create_quiet_dummy();
	if (!sub1 || !sub2)
		return (tc_module_destroy(root), 1);
	tc_module_add_sub(root, sub1);
	tc_module_add_sub(root, sub2);
	tc_module_taddf(root, dummy_success_test, "r1");
	tc_module_run(root);
	tc_module_display(root, TC_DISPLAY_DEFAULT);
	tc_module_display(root, TC_DSBM_SEP);
	tc_module_display(root, TC_DSBM_SEP | TC_DM_SUMNBS);
	tc_module_display(root, TC_DSBM_SEP | TC_DSBM_SUMPCT);
	tc_module_display(root, TC_DSBM_SUMPCT);
	tc_module_display(root, TC_DM_SUMSILENT);
	tc_module_display(root, TC_FMT_JSN);
	tc_module_display(root, TC_FMT_JSN | TC_DSBM_SEP);
	tc_module_destroy(root);
	tc_module_init(&root, "parent", "no direct tests");
	sub1 = create_quiet_dummy();
	if (!sub1)
		return (tc_module_destroy(root), 2);
	tc_module_add_sub(root, sub1);
	tc_module_run(root);
	tc_module_display(root, TC_DSBM_SEP | TC_DM_SUMNBS);
	return (tc_module_destroy(root), EXIT_SUCCESS);
}

static int	display_internal(void)
{
	t_module	*root;
	t_module	*sub;

	tc_module_init(&root, "root", "root description");
	sub = create_quiet_dummy();
	if (!sub)
		return (tc_module_destroy(root), 1);
	tc_module_add_sub(root, sub);
	tci_display_results_json(root, TC_FMT_JSN | TC_DM_SUMSILENT);
	tci_mdisplay_sum_nb(1, 1, TC_FMT_SILENT);
	tci_mdisplay_sum_percent(1, 1, TC_FMT_SILENT);
	tci_mdisplay_sbm(root->submodules_list, TC_FMT_JSN);
	tc_module_destroy(root);
	return (EXIT_SUCCESS);
}

static int	display_guards(void)
{
	t_module	*root;

	root = create_quiet_dummy();
	tc_module_display(NULL, TC_DISPLAY_DEFAULT);
	tc_module_display(root, TC_FMT_SILENT);
	tc_module_display(root, TC_FMT_JSN | TC_DM_SUMSILENT);
	tc_module_display(root, (t_display_mode)(2 << 6));
	tc_module_display(root, TC_DM_SUMPCT);
	tc_module_display(root, TC_DM_SUMPCT | TC_PRTY_NOCOLOR);
	tc_module_destroy(root);
	return (EXIT_SUCCESS);
}

int	tcit_module_display_extra(void)
{
	int	ret;

	ret = display_empty();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = display_results();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = display_submodules();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = display_internal();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = display_guards();
	return (ret);
}
