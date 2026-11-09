#include "ct_capture.h"
#include "ct_memory.h"
#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_modules.h"
#include "ct_utils.h"

// ct_module_display(empty, CT_DM_SUMPCT);
#define EMPTY_CALL1                                                            \
	"empty\tno tests\n"                                                        \
	"empty\tNaN%\n"

// ct_module_display(empty, CT_DM_SUMPCT | CT_FMT_JSN);
#define EMPTY_CALL2                                                            \
	"{\"module_name\":\"empty\",\"description\":"                              \
	"\"no tests\",\"tests\":[],\"submodules\":[],\"percentage\":\"NaN%\"}\n"

// ct_module_display(empty, CT_DM_SUMNBS);
#define EMPTY_CALL3                                                            \
	"empty\tno tests\n"                                                        \
	"empty\tempty\n"

// ct_module_display(empty, CT_DISPLAY_DEFAULT);
#define EMPTY_CALL4                                                            \
	"empty\tno tests\n"                                                        \
	"empty\tempty\n"

//	ct_module_display(fail, CT_DT_OK);
#define ERR_CALL1                                                              \
	"fail\tfailing results\n"                                                  \
	"\tcrash  \033[31mKO\033[0m\t139\n"                                        \
	"\tko     \033[31mKO\033[0m\t1\n"                                          \
	"fail\t\033[31m0\033[0m / 2\n"

//	ct_module_display(fail, CT_FMT_JSN);
#define ERR_CALL2                                                              \
	"{\"module_name\":\"fail\",\"description\":\"failing "                     \
	"results\",\"tests\":[{\"test_name\":\"crash\",\"result\":\"KO\","         \
	"\"return_value\":\"139\",\"timed_out\":\"false\",\"signaled\":\"true\"}," \
	"{\"test_name\":\"ko\",\"result\":\"KO\",\"return_value\":\"1\",\"timed_"  \
	"out\":\"false\",\"signaled\":\"true\"}],\"submodules\":[],\"nb_tests_"    \
	"passed\":\"0\",\"nb_tests_total\":\"2\"}\n"

//	ct_module_display(fail, CT_FMT_JSN | CT_DT_OK);
#define ERR_CALL3                                                              \
	"{\"module_name\":\"fail\",\"description\":\"failing "                     \
	"results\",\"tests\":[{\"test_name\":\"crash\",\"result\":\"KO\","         \
	"\"return_value\":\"139\",\"timed_out\":\"false\",\"signaled\":\"true\"}," \
	"{\"test_name\":\"ko\",\"result\":\"KO\",\"return_value\":\"1\",\"timed_"  \
	"out\":\"false\",\"signaled\":\"true\"}],\"submodules\":[],\"nb_tests_"    \
	"passed\":\"0\",\"nb_tests_total\":\"2\"}\n"

//	ct_module_display(root, CT_DISPLAY_DEFAULT);
#define SUB_CALL1                                                              \
	"root\troot description\n"                                                 \
	"\tstr\ttests for dummy string module\n"                                   \
	"\tstr\t\033[32m1\033[0m / 1\n"                                            \
	"\tstr\ttests for dummy string module\n"                                   \
	"\tstr\t\033[32m1\033[0m / 1\n"                                            \
	"root\t\033[32m3\033[0m / 3\t[str: \033[32m1\033[0m / 1][str: "            \
	"\033[32m1\033[0m / 1]\n"

//	ct_module_display(root, CT_DSBM_SEP);
#define SUB_CALL2                                                              \
	"root\troot description\n"                                                 \
	"\tstr\ttests for dummy string module\n"                                   \
	"\tstr\n"                                                                  \
	"\tstr\ttests for dummy string module\n"                                   \
	"\tstr\n"                                                                  \
	"root\t\033[32m1\033[0m / 1\t[str: \033[32m1\033[0m / 1][str: "            \
	"\033[32m1\033[0m / 1]\n"

//	ct_module_display(root, CT_DSBM_SEP | CT_DM_SUMNBS);
#define SUB_CALL3                                                              \
	"root\troot description\n"                                                 \
	"\tstr\ttests for dummy string module\n"                                   \
	"\tstr\n"                                                                  \
	"\tstr\ttests for dummy string module\n"                                   \
	"\tstr\n"                                                                  \
	"root\t\033[32m1\033[0m / 1\t[str: \033[32m1\033[0m / 1][str: "            \
	"\033[32m1\033[0m / 1]\n"

//	ct_module_display(root, CT_DSBM_SEP | CT_DSBM_SUMPCT);
#define SUB_CALL4                                                              \
	"root\troot description\n"                                                 \
	"\tstr\ttests for dummy string module\n"                                   \
	"\tstr\n"                                                                  \
	"\tstr\ttests for dummy string module\n"                                   \
	"\tstr\n"                                                                  \
	"root\t\033[32m1\033[0m / 1\t[str: (\033[32m100.00\033[0m%)][str: "        \
	"(\033[32m100.00\033[0m%)]\n"

//	ct_module_display(root, CT_DSBM_SUMPCT);
#define SUB_CALL5                                                              \
	"root\troot description\n"                                                 \
	"\tstr\ttests for dummy string module\n"                                   \
	"\tstr\t\033[32m1\033[0m / 1\n"                                            \
	"\tstr\ttests for dummy string module\n"                                   \
	"\tstr\t\033[32m1\033[0m / 1\n"                                            \
	"root\t\033[32m3\033[0m / 3\t[str: (\033[32m100.00\033[0m%)][str: "        \
	"(\033[32m100.00\033[0m%)]\n"

// fmt type not specified, aka 0 aka txt, txt+silent = "", just evaluate unit
// tests with the returned value from main.
//	ct_module_display(root, CT_DM_SUMSILENT);
#define SUB_CALL6 ""

//	ct_module_display(root, CT_FMT_JSN);
#define SUB_CALL7                                                              \
	"{\"module_name\":\"root\","                                               \
	"\"description\":\"root description\","                                    \
	"\"tests\":["                                                              \
	"{\"test_name\":\"r1\",\"result\":\"OK\",\"return_value\":\"0\","          \
	"\"timed_out\":\"false\",\"signaled\":\"false\"}"                          \
	"],"                                                                       \
	"\"submodules\":["                                                         \
	"{\"module_name\":\"str\",\"description\":\"tests for dummy string "       \
	"module\","                                                                \
	"\"tests\":["                                                              \
	"{\"test_name\":\"ss1\",\"result\":\"OK\",\"return_value\":\"0\","         \
	"\"timed_out\":\"false\",\"signaled\":\"false\"}"                          \
	"],"                                                                       \
	"\"submodules\":[],"                                                       \
	"\"nb_tests_passed\":\"1\",\"nb_tests_total\":\"1\"},"                     \
	"{\"module_name\":\"str\",\"description\":\"tests for dummy string "       \
	"module\","                                                                \
	"\"tests\":["                                                              \
	"{\"test_name\":\"ss1\",\"result\":\"OK\",\"return_value\":\"0\","         \
	"\"timed_out\":\"false\",\"signaled\":\"false\"}"                          \
	"],"                                                                       \
	"\"submodules\":[],"                                                       \
	"\"nb_tests_passed\":\"1\",\"nb_tests_total\":\"1\"}"                      \
	"],"                                                                       \
	"\"nb_tests_passed\":\"3\",\"nb_tests_total\":\"3\"}\n"

//	ct_module_display(root, CT_FMT_JSN | CT_DSBM_SEP);
#define SUB_CALL8                                                              \
	"{\"module_name\":\"root\",\"description\":"                               \
	"\"root description\","                                                    \
	"\"tests\":["                                                              \
	"{\"test_name\":\"r1\",\"result\":\"OK\",\"return_value\":\"0\","          \
	"\"timed_out\":\"false\",\"signaled\":\"false\"}"                          \
	"],"                                                                       \
	"\"submodules\":["                                                         \
	"{\"module_name\":\"str\","                                                \
	"\"description\":\"tests for dummy string module\","                       \
	"\"tests\":["                                                              \
	"{\"test_name\":\"ss1\",\"result\":\"OK\",\"return_value\":\"0\","         \
	"\"timed_out\":\"false\",\"signaled\":\"false\"}"                          \
	"],"                                                                       \
	"\"submodules\":[],"                                                       \
	"\"nb_tests_passed\":\"1\",\"nb_tests_total\":\"1\"},"                     \
	"{\"module_name\":\"str\","                                                \
	"\"description\":\"tests for dummy string module\","                       \
	"\"tests\":["                                                              \
	"{\"test_name\":\"ss1\",\"result\":\"OK\",\"return_value\":\"0\","         \
	"\"timed_out\":\"false\",\"signaled\":\"false\"}"                          \
	"],"                                                                       \
	"\"submodules\":[],"                                                       \
	"\"nb_tests_passed\":\"1\",\"nb_tests_total\":\"1\"}"                      \
	"],"                                                                       \
	"\"nb_tests_passed\":\"1\",\"nb_tests_total\":\"1\"}\n"

// cti_display_results_json(root, CT_FMT_JSN | CT_DM_SUMSILENT);
#define INTER_CALL1                                                            \
	"{\"module_name\":\"root\","                                               \
	"\"description\":\"root description\","                                    \
	"\"tests\":[],"                                                            \
	"\"submodules\":["                                                         \
	"{\"module_name\":\"str\","                                                \
	"\"description\":\"tests for dummy string module\","                       \
	"\"tests\":["                                                              \
	"{\"test_name\":\"ss1\",\"result\":\"OK\",\"return_value\":\"0\","         \
	"\"timed_out\":\"false\",\"signaled\":\"false\"}"                          \
	"],"                                                                       \
	"\"submodules\":[]}"                                                       \
	"]"                                                                        \
	"}"

// cti_mdisplay_sum_nb(1, 1, CT_FMT_SILENT);
#define INTER_CALL2 "Unsupported format for percentage display\n"

// cti_mdisplay_sum_percent(1, 1, CT_FMT_SILENT);
#define INTER_CALL3 "Unsupported format for percentage display\n"

// cti_mdisplay_sbm(root->submodules_list, CT_FMT_JSN);
#define INTER_CALL4 "Unsupported format for submodule display\n"

// collated expected output for empty tests
#define E_EMPTY                                                                \
	EMPTY_CALL1                                                                \
	EMPTY_CALL2                                                                \
	EMPTY_CALL3                                                                \
	EMPTY_CALL4

// collated expected output for error tests
#define E_RESULTS                                                              \
	ERR_CALL1                                                                  \
	ERR_CALL2                                                                  \
	ERR_CALL3

// expected output of the first half of display_submodules
#define E_SUBMODULES                                                           \
	SUB_CALL1                                                                  \
	SUB_CALL2                                                                  \
	SUB_CALL3                                                                  \
	SUB_CALL4                                                                  \
	SUB_CALL5                                                                  \
	SUB_CALL6                                                                  \
	SUB_CALL7                                                                  \
	SUB_CALL8

// expected output of the second half of display_submodules
#define E_PARENT                                                               \
	"parent\tno direct tests\n"                                                \
	"\tstr\ttests for dummy string module\n"                                   \
	"\tstr\n"

// expected output of display_internal
#define E_INTERNAL                                                             \
	INTER_CALL1                                                                \
	INTER_CALL2                                                                \
	INTER_CALL3                                                                \
	INTER_CALL4

// expected output of display_guards
#define E_GUARDS                                                               \
	"ctests: json format cannot skip any field.\n"                             \
	"unsuported format\n"                                                      \
	"str\ttests for dummy string module\n"                                     \
	"str\t(\033[31m0.00\033[0m%)\n"                                            \
	"str\ttests for dummy string module\n"                                     \
	"str\t(0.00\n"

static int check_capture(t_capture *c, const char *exp)
{
	char *s;
	int	  ret;

	s = ct_capture_get(c);
	ct_capture_destroy(c);
	if (!s)
		return (EXIT_FAILURE);
	ret = EXIT_SUCCESS;
	if (ct_memcmp(s, exp, ct_strlen(exp) + 1) != 0)
		ret = EXIT_FAILURE;
	return (safefree(s), ret);
}

static int display_empty(void)
{
	t_module  *empty;
	t_capture *c;
	int		   ret;

	ct_module_init(&empty, "empty", "no tests");
	c = ct_capture_create(STDOUT_FILENO);
	if (!c)
		return (ct_module_destroy(empty), EXIT_FAILURE);
	ct_module_display(empty, CT_DM_SUMPCT);
	ct_module_display(empty, CT_DM_SUMPCT | CT_FMT_JSN);
	ct_module_display(empty, CT_DM_SUMNBS);
	ct_module_display(empty, CT_DISPLAY_DEFAULT);
	ret = check_capture(c, E_EMPTY);
	ct_module_destroy(empty);
	return (ret);
}

static int display_results(void)
{
	t_module  *fail;
	t_capture *c;
	int		   ret;

	ct_module_init(&fail, "fail", "failing results");
	ct_module_taddf(fail, dummy_fail_test, "ko");
	ct_module_taddf(fail, dummy_sigsegv, "crash");
	ct_module_run(fail);
	c = ct_capture_create(STDOUT_FILENO);
	if (!c)
		return (ct_module_destroy(fail), EXIT_FAILURE);
	ct_module_display(fail, CT_DT_OK);
	ct_module_display(fail, CT_FMT_JSN);
	ct_module_display(fail, CT_FMT_JSN | CT_DT_OK);
	ret = check_capture(c, E_RESULTS);
	ct_module_destroy(fail);
	return (ret);
}

static int display_submodules(void)
{
	t_module  *root;
	t_module  *sub1;
	t_module  *sub2;
	t_capture *c;
	int		   ret;

	ct_module_init(&root, "root", "root description");
	sub1 = create_quiet_dummy();
	sub2 = create_quiet_dummy();
	if (!sub1 || !sub2)
		return (ct_module_destroy(root), EXIT_FAILURE);
	ct_module_add_sub(root, sub1);
	ct_module_add_sub(root, sub2);
	ct_module_taddf(root, dummy_success_test, "r1");
	ct_module_run(root);
	c = ct_capture_create(STDOUT_FILENO);
	if (!c)
		return (ct_module_destroy(root), EXIT_FAILURE);
	ct_module_display(root, CT_DISPLAY_DEFAULT);
	ct_module_display(root, CT_DSBM_SEP);
	ct_module_display(root, CT_DSBM_SEP | CT_DM_SUMNBS);
	ct_module_display(root, CT_DSBM_SEP | CT_DSBM_SUMPCT);
	ct_module_display(root, CT_DSBM_SUMPCT);
	ct_module_display(root, CT_DM_SUMSILENT);
	ct_module_display(root, CT_FMT_JSN);
	ct_module_display(root, CT_FMT_JSN | CT_DSBM_SEP);
	ret = check_capture(c, E_SUBMODULES);
	ct_module_destroy(root);
	if (ret != EXIT_SUCCESS)
		return (ret);
	ct_module_init(&root, "parent", "no direct tests");
	sub1 = create_quiet_dummy();
	if (!sub1)
		return (ct_module_destroy(root), EXIT_FAILURE);
	ct_module_add_sub(root, sub1);
	ct_module_run(root);
	c = ct_capture_create(STDOUT_FILENO);
	if (!c)
		return (ct_module_destroy(root), EXIT_FAILURE);
	ct_module_display(root, CT_DSBM_SEP | CT_DM_SUMNBS);
	ret = check_capture(c, E_PARENT);
	return (ct_module_destroy(root), ret);
}

static int display_internal(void)
{
	t_module  *root;
	t_module  *sub;
	t_capture *c;
	int		   ret;

	ct_module_init(&root, "root", "root description");
	sub = create_quiet_dummy();
	if (!sub)
		return (ct_module_destroy(root), EXIT_FAILURE);
	ct_module_add_sub(root, sub);
	c = ct_capture_create(STDOUT_FILENO);
	if (!c)
		return (ct_module_destroy(root), EXIT_FAILURE);
	cti_display_results_json(root, CT_FMT_JSN | CT_DM_SUMSILENT);
	cti_mdisplay_sum_nb(1, 1, CT_FMT_SILENT);
	cti_mdisplay_sum_percent(1, 1, CT_FMT_SILENT);
	cti_mdisplay_sbm(root->submodules_list, CT_FMT_JSN);
	ret = check_capture(c, E_INTERNAL);
	ct_module_destroy(root);
	return (ret);
}

static int display_guards(void)
{
	t_module  *root;
	t_capture *c;
	int		   ret;

	root = create_quiet_dummy();
	c = ct_capture_create(STDOUT_FILENO);
	if (!c)
		return (ct_module_destroy(root), EXIT_FAILURE);
	ct_module_display(NULL, CT_DISPLAY_DEFAULT);
	ct_module_display(root, CT_FMT_SILENT);
	ct_module_display(root, CT_FMT_JSN | CT_DM_SUMSILENT);
	ct_module_display(root, (t_display_mode)(2 << 6));
	ct_module_display(root, CT_DM_SUMPCT);
	ct_module_display(root, CT_DM_SUMPCT | CT_PRTY_NOCOLOR);
	ret = check_capture(c, E_GUARDS);
	ct_module_destroy(root);
	return (ret);
}

int ctit_module_display_extra(void)
{
	int ret;

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
