#include "ct_capture.h"
#include "ct_memory.h"
#include "ct_structs.h"
#include <stdio.h>
#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_modules.h"
#include "ct_utils.h"

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

int	ctit_module_display(void)
{
	t_module	*md;
	t_capture	*c;
	int			ret;
	char		*s;
	const char	*exp = get_expected();

	md = create_quiet_dummy();
	ct_module_run(md);
	c = ct_capture_create(STDOUT_FILENO);
	ct_module_sort(md);
	ct_module_display(md, CT_DISPLAY_DEFAULT | CT_PRTY_NOCOLOR);
	ct_module_display(md, CT_DISPLAY_DEFAULT);
	ct_module_display(md, CT_FMT_JSN);
	ct_module_display(md, CT_FMT_JSN | CT_DM_SUMPCT);
	ct_module_display(md, CT_FMT_JSN | CT_DT_SHRTNAME);
	ct_module_display(md, CT_FMT_JSN | CT_DT_FULLNAME);
	ct_module_display(md, CT_DT_SHRTNAME | CT_DT_OK);
	ct_module_display(md, CT_DT_FULLNAME | CT_DT_OK);
	ret = EXIT_SUCCESS;
	s = ct_capture_get(c);
	ct_capture_destroy(c);
	if (ct_memcmp(s, exp, ct_strlen(exp) + 1) != 0)
		ret = 1;
	return (safefree(s), ct_module_destroy(md), ret);
}
