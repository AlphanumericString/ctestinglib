#define CTESTS_INTERNAL_TESTS
#include "ct_structs.h"
#include "ct_modules.h"
#include "ct_utils.h"

#define TCMODDESCS "Check on handling utils functionalities"

static t_ftest	*load_tests_list(void)
{
	static t_ftest	tests[] = {
	{"memset", ctit_memset}, {"memcmp", ctit_memcmp}, {"memcpy", ctit_memcpy},
	{"memmove", ctit_memmove}, {"memchr", ctit_memchr},
	{"strncmp", ctit_strncmp}, {"strlen", ctit_strlen},
	{"list_push", ctit_listpush}, {"list_pop", ctit_listpop},
	{"list_foreach", ctit_listforeach}, {"cmp_alpha", ctit_cmpalpha},
	{"print_charn", ctit_print_cn}, {"print_nb", ctit_print_nb},
	{"print_flt", ctit_print_flt}, {"print_str", ctit_print_string},
	{0, 0}
	};

	return (tests);
}

t_module	*create_utils_tests(void)
{
	size_t			i;
	t_module		*util_module;
	const t_ftest	*tests = load_tests_list();

	ct_module_init(&util_module, "util", TCMODDESCS);
	i = 0;
	while (tests[i].name)
	{
		ct_module_taddf(util_module, tests[i].test_func, tests[i].name);
		i++;
	}
	return (util_module);
}

#undef TCMODDESCS
