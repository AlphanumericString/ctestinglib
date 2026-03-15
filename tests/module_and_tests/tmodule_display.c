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
#include "tc_structs.h"
#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "tc_modules.h"
#include "tc_utils.h"

#define EXP		"str\ttests for dummy string module\nstr\t1 / 1\n"
#define EXP2	"str\ttests for dummy string module\nstr\t"
#define EXP2_	" / 1\n"
#define EXP3	"{\"module_name\":\"str\",\"description\":\"tests for dummy "
#define EXP3_	"string module\",\"tests\":[],\"submodules\":[],\""
#define EXP4	"nb_tests_passed\":\"1\",\"nb_tests_total\":\"1\"}"

int	tcit_module_display(void)
{
	t_module	*md;
	t_capture	*c;
	int			ret;
	char		*s;
	const char	*exp = EXP EXP2 OK_COLOR "1" RESET_COLOR EXP2_ EXP3 EXP3_ EXP4;

	md = create_string_tests();
	tc_module_run(md);
	c = tc_capture_create(STDOUT_FILENO);
	tc_module_sort(md);
	tc_module_display(md, TC_DISPLAY_DEFAULT | TC_PRTY_NOCOLOR);
	tc_module_display(md, TC_DISPLAY_DEFAULT);
	tc_module_display(md, TC_DISPLAY_DEFAULT | TC_FMT_JSN);
	ret = EXIT_SUCCESS;
	s = tc_capture_get(c);
	tc_capture_destroy(c);
	if (tc_memcmp(s, exp, tc_strlen(exp) + 1) != 0)
	{
		ret = 1;
	}
	tc_module_destroy(md);
	return (ret);
}
