/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cm_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 12:46:29 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/03 09:12:14 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CTESTS_INTERNAL_TESTS
#include "tc_structs.h"
#include "tc_modules.h"
#include "tc_utils.h"

#define TCMODDESCS "Check on handling utils functionalities"

static t_ftest	*load_tests_list(void)
{
	static t_ftest	tests[] = {
	{"memset", tcit_memset}, {"memcmp", tcit_memcmp}, {"memcpy", tcit_memcpy},
	{"memmove", tcit_memmove}, {"memchr", tcit_memchr},
	{"strncmp", tcit_strncmp}, {"strlen", tcit_strlen},
	{"list_push", tcit_listpush}, {"list_pop", tcit_listpop},
	{"list_foreach", tcit_listforeach}, {"cmp_alpha", tcit_cmpalpha},
	{"print_charn", tcit_print_cn}, {"print_nb", tcit_print_nb},
	{"print_flt", tcit_print_flt}, {"print_str", tcit_print_string},
	{0, 0}
	};

	return (tests);
}

t_module	*create_utils_tests(void)
{
	size_t			i;
	t_module		*util_module;
	const t_ftest	*tests = load_tests_list();

	tc_module_init(&util_module, "util", TCMODDESCS);
	i = 0;
	while (tests[i].name)
	{
		tc_module_taddf(util_module, tests[i].test_func, tests[i].name);
		i++;
	}
	return (util_module);
}

#undef TCMODDESCS
