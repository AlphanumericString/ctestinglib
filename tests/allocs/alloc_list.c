/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 13:18:17 by bgoulard          #+#    #+#             */
/*   Updated: 2026/01/22 15:15:34 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "tc_memory.h"
#include "tc_modules.h"
#include "tc_utils.h"

static int	g_custom_free_calls;

static void	custom_free(void *p)
{
	g_custom_free_calls++;
	free(p);
}

static void	auto_call_helper(void)
{
	void	*a;
	void	*b;

	a = malloc(8);
	b = malloc(8);
	if (a)
		free(a);
	if (b)
		free(b);
}

int	tcit_cleanup(void)
{
	t_module	*tmp_dummy;
	void		*a;
	void		*b;

	tci_set_memory_cleanup(true);
	a = malloc(32);
	b = malloc(32);
	if (!a || !b)
		return (tci_set_memory_cleanup(false), 1);
	do_cleanup(NULL);
	tci_set_memory_cleanup(false);
	a = safealloc(32);
	b = safealloc(32);
	if (!a || !b)
		return (safefree(a), safefree(b), 2);
	push_to_cleanup(a);
	push_to_cleanup(b);
	g_custom_free_calls = 0;
	do_cleanup(custom_free);
	if (g_custom_free_calls != 2)
		return (3);
	do_cleanup(NULL);
	tc_module_init(&tmp_dummy, "module_dummy", "dummy module to test cleanups");
	tc_module_taddf(tmp_dummy, dummy_alloc, "alloc");
	tmp_dummy->auto_cleanup = true;
	tc_module_run(tmp_dummy);
	tc_module_destroy(tmp_dummy);
	return (EXIT_SUCCESS);
}

int	tcit_count(void)
{
	size_t	before;
	void	*p;

	before = tc_alloc_getcount();
	p = malloc(64);
	if (!p)
		return (1);
	if (tc_alloc_getcount() != before + 1)
		return (free(p), 2);
	free(p);
	if (tci_alloc_get() == NULL || tci_failloc_get() == NULL)
		return (3);
	return (EXIT_SUCCESS);
}

int	tcit_fail(void)
{
	void	*p;

	tc_failloc_reset();
	tc_failloc_set(0);
	if (malloc(16) != NULL)
		return (tc_failloc_reset(), 1);
	tc_failloc_set(1);
	p = malloc(16);
	if (!p)
		return (tc_failloc_reset(), 2);
	free(p);
	if (malloc(16) != NULL)
		return (tc_failloc_reset(), 3);
	tc_failloc_reset();
	p = malloc(16);
	if (p == NULL)
		return (4);
	free(p);
	tc_auto_call(auto_call_helper);
	tc_failloc_reset();
	if (tc_alloc_getcount() == 0)
		return (5);
	return (EXIT_SUCCESS);
}
