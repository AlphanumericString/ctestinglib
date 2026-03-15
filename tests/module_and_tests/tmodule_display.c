/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmodule_display.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 22:14:41 by bgoulard          #+#    #+#             */
/*   Updated: 2026/01/08 22:14:41 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tc_capture.h"
#include "tc_memory.h"
#include "tc_structs.h"
#include <stdio.h>
#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "tc_modules.h"
#include "tc_utils.h"

// text format 1
#define T_E1_1	"str\ttests for dummy string module\nstr\t1 / 1\n"

// text format 2
#define T_E2_1	"str\ttests for dummy string module\nstr\t"
#define T_E2_2	"\033[32m1\033[0m / 1\n"

// json format 1
#define J_E1_1	"{\"module_name\":\"str\",\"description\":\"tests for dummy "
#define J_E1_2	"string module\",\"tests\":[{\"test_name\":\"ss1\",\"result\":"
#define J_E1_3	"\"OK\",\"return_value\":\"0\",\"signaled\":\"false\"}],\"subm"
#define J_E1_4	"odules\":[],\"nb_tests_passed\":\"1\",\"nb_tests_total\":"
#define J_E1_5	"\"1\"}\n"

// json format 2
#define J_E2_1	"{\"module_name\":\"str\",\"description\":\"tests for dummy st"
#define J_E2_2	"ring module\",\"tests\":[{\"test_name\":\"ss1\",\"result\":\""
#define J_E2_3	"OK\",\"return_value\":\"0\",\"signaled\":\"false\"}],\"submod"
#define J_E2_4	"ules\":[],\"percentage\":\"100.00%\"}\n"

// json format 3
#define J_E3_1	"{\"module_name\":\"str\",\"description\":\"tests for dummy st"
#define J_E3_2	"ring module\",\"tests\":[{\"test_name\":\"ss1\",\"result\":\""
#define J_E3_3	"OK\",\"return_value\":\"0\",\"signaled\":\"false\"}],\"submod"
#define J_E3_4	"ules\":[],\"nb_tests_passed\":\"1\",\"nb_tests_total\":\"1\"}"
#define J_E3_5	"\n"

// json format 4
#define J_E4_1	"{\"module_name\":\"str\",\"description\":\"tests for dummy st"
#define J_E4_2	"ring module\",\"tests\":[{\"test_name\":\"str:ss1\",\"result"
#define J_E4_3	"\":\"OK\",\"return_value\":\"0\",\"signaled\":\"false\"}],\"s"
#define J_E4_4	"ubmodules\":[],\"nb_tests_passed\":\"1\",\"nb_tests_total\":"
#define J_E4_5	"\"1\"}\n"

// text format 3
#define T_E3_1	"str\ttests for dummy string module\n\tss1  \033[32mOK\033[0m"
#define T_E3_2	"\nstr\t\033[32m1\033[0m / 1\n"

// text format 4
#define T_E4_1	"str\ttests for dummy string module\n\tstr:ss1  \033[32mOK\033"
#define T_E4_2	"[0m\nstr\t\033[32m1\033[0m / 1\n"

static const char	*get_expected(void)
{
	return (T_E1_1
		T_E2_1 T_E2_2
		J_E1_1 J_E1_2 J_E1_3 J_E1_4 J_E1_5
		J_E2_1 J_E2_2 J_E2_3 J_E2_4
		J_E3_1 J_E3_2 J_E3_3 J_E3_4 J_E3_5
		J_E4_1 J_E4_2 J_E4_3 J_E4_4 J_E4_5
		T_E3_1 T_E3_2
		T_E4_1 T_E4_2
	);
}

int	tcit_module_display(void)
{
	t_module	*md;
	t_capture	*c;
	int			ret;
	char		*s;
	const char	*exp = get_expected();

	md = create_quiet_dummy();
	tc_module_run(md);
	c = tc_capture_create(STDOUT_FILENO);
	tc_module_sort(md);
	tc_module_display(md, TC_DISPLAY_DEFAULT | TC_PRTY_NOCOLOR);
	tc_module_display(md, TC_DISPLAY_DEFAULT);
	tc_module_display(md, TC_FMT_JSN);
	tc_module_display(md, TC_FMT_JSN | TC_DM_SUMPCT);
	tc_module_display(md, TC_FMT_JSN | TC_DT_SHRTNAME);
	tc_module_display(md, TC_FMT_JSN | TC_DT_FULLNAME);
	tc_module_display(md, TC_DT_SHRTNAME | TC_DT_OK);
	tc_module_display(md, TC_DT_FULLNAME | TC_DT_OK);
	ret = EXIT_SUCCESS;
	s = tc_capture_get(c);
	tc_capture_destroy(c);
	if (tc_memcmp(s, exp, tc_strlen(exp) + 1) != 0)
		ret = 1;
	safefree(s);
	tc_module_destroy(md);
	return (ret);
}
