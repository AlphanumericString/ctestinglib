#include <stdlib.h>
#include <string.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_internal.h"

#define HTML_DOC_OPEN                                                          \
	"<!DOCTYPE html>\n"                                                        \
	"<html lang=\"en\">\n"                                                     \
	"<head>\n"                                                                 \
	"<meta charset=\"utf-8\">\n"                                               \
	"<title>ctests report</title>\n"                                           \
	"</head>\n"                                                                \
	"<body>\n"                                                                 \
	"<main class=\"ctests\">\n"

#define HTML_DOC_CLOSE "</main>\n</body>\n</html>\n"

#define HTML_MODULE_OPEN(depth, name, desc)                                    \
	"<section class=\"module\" data-depth=\"" depth "\">\n"                    \
	"<header class=\"module-header\">\n"                                       \
	"<h2>" name "</h2>\n"                                                      \
	"<p class=\"module-description\">" desc "</p>\n"                           \
	"</header>\n"

#define HTML_MODULE_CLOSE "</section>\n"

#define HTML_TESTS_OPEN "<ul class=\"tests\">\n"
#define HTML_TESTS_CLOSE "</ul>\n"

#define HTML_SUBMODULES_OPEN "<div class=\"submodules\">\n"
#define HTML_SUBMODULES_CLOSE "</div>\n"

#define HTML_TEST_OK(name)                                                     \
	"<li class=\"test ok\">\n"                                                 \
	"<span class=\"test-name\">" name "</span>\n"                              \
	"<span class=\"test-result\">OK</span>\n"                                  \
	"<span class=\"test-return-value\">0</span>\n"                             \
	"<span class=\"test-timed-out\">false</span>\n"                            \
	"<span class=\"test-signaled\">false</span>\n"                             \
	"</li>\n"

#define HTML_TEST_KO(name, ret, timed, signaled)                               \
	"<li class=\"test ko\">\n"                                                 \
	"<span class=\"test-name\">" name "</span>\n"                              \
	"<span class=\"test-result\">KO</span>\n"                                  \
	"<span class=\"test-return-value\">" ret "</span>\n"                       \
	"<span class=\"test-timed-out\">" timed "</span>\n"                        \
	"<span class=\"test-signaled\">" signaled "</span>\n"                      \
	"</li>\n"

#define HTML_SUMMARY_OPEN "<footer class=\"summary\">\n"
#define HTML_SUMMARY_CLOSE "</footer>\n"

#define HTML_NB_LINE(passed, total)                                            \
	"<span class=\"summary-nb\">" passed " / " total "</span>\n"

#define HTML_PCT_LINE(pct) "<span class=\"summary-percent\">" pct "</span>\n"

#define HTML_SUMMARY_NB(passed, total)                                         \
	HTML_SUMMARY_OPEN HTML_NB_LINE(passed, total) HTML_SUMMARY_CLOSE

#define HTML_SUMMARY_PCT(pct)                                                  \
	HTML_SUMMARY_OPEN HTML_PCT_LINE(pct) HTML_SUMMARY_CLOSE

#define HTML_SBM_RECAP_PCT(name, pct)                                          \
	"<span class=\"submodule\">"                                               \
	"<span class=\"submodule-name\">" name "</span>"                           \
	"<span class=\"summary-percent\">" pct "</span></span>\n"

static int check_capture(t_capture *c, const char *exp)
{
	char *s;
	int	  ret;

	s = ct_capture_get(c);
	ct_capture_destroy(c);
	if (!s)
		return (EXIT_FAILURE);
	ret = EXIT_SUCCESS;
	if (strcmp(s, exp) != 0)
		ret = EXIT_FAILURE;
	return (safefree(s), ret);
}

// a standalone html document wrapping one module and its tests
static int display_html_document(void)
{
	t_module   *str;
	t_capture  *c;
	int			ret;
	const char *exp;

	ct_module_init(&str, "str", "tests for dummy string module");
	ct_module_taddf(str, dummy_success_test, "ss1");
	ct_module_run(str);
	c = ct_capture_create(STDOUT_FILENO);
	if (!c)
		return (ct_module_destroy(str), EXIT_FAILURE);
	ct_module_display(str, CT_FMT_HTML | CT_PRTY_NOCOLOR);
	exp = HTML_DOC_OPEN HTML_MODULE_OPEN("0", "str",
										 "tests for dummy string module")
		HTML_TESTS_OPEN HTML_TEST_OK("ss1")
			HTML_TESTS_CLOSE HTML_SUBMODULES_OPEN HTML_SUBMODULES_CLOSE
				HTML_SUMMARY_NB("1", "1") HTML_MODULE_CLOSE HTML_DOC_CLOSE;
	ret = check_capture(c, exp);
	ct_module_destroy(str);
	return (ret);
}

// failing tests expose ko rows, return values and timeouts
static int display_html_results(void)
{
	t_module   *fail;
	t_capture  *c;
	int			ret;
	const char *exp;

	ct_module_init(&fail, "fail", "failing results");
	ct_module_taddf_timeout(fail, dummy_timeout, "timeout", 1);
	ct_module_taddf(fail, dummy_fail_test, "ko");
	ct_module_taddf(fail, dummy_sigsegv, "crash");
	ct_module_run(fail);
	c = ct_capture_create(STDOUT_FILENO);
	if (!c)
		return (ct_module_destroy(fail), EXIT_FAILURE);
	ct_module_display(fail, CT_FMT_HTML | CT_PRTY_NOCOLOR);
	exp = HTML_DOC_OPEN HTML_MODULE_OPEN("0", "fail", "failing results")
		HTML_TESTS_OPEN HTML_TEST_KO("crash", "139", "false", "true")
			HTML_TEST_KO("ko", "1", "false", "true")
				HTML_TEST_KO("timeout", "9", "true", "true")
					HTML_TESTS_CLOSE HTML_SUBMODULES_OPEN HTML_SUBMODULES_CLOSE
											  HTML_SUMMARY_NB("0", "3")
							HTML_MODULE_CLOSE HTML_DOC_CLOSE;
	ret = check_capture(c, exp);
	ct_module_destroy(fail);
	return (ret);
}

// empty modules keep a valid summary, percentage is NaN when 0/0
static int display_html_empty(void)
{
	t_module   *empty;
	t_capture  *c;
	int			ret;
	const char *exp;

	ct_module_init(&empty, "empty", "no tests");
	c = ct_capture_create(STDOUT_FILENO);
	if (!c)
		return (ct_module_destroy(empty), EXIT_FAILURE);
	ct_module_display(empty, CT_FMT_HTML | CT_DM_SUMNBS | CT_PRTY_NOCOLOR);
	ct_module_display(empty, CT_FMT_HTML | CT_DM_SUMPCT | CT_PRTY_NOCOLOR);
	exp = HTML_DOC_OPEN HTML_MODULE_OPEN("0", "empty", "no tests")
		HTML_TESTS_OPEN HTML_TESTS_CLOSE HTML_SUBMODULES_OPEN
			HTML_SUBMODULES_CLOSE HTML_SUMMARY_NB("0", "0")
				HTML_MODULE_CLOSE HTML_DOC_CLOSE HTML_DOC_OPEN HTML_MODULE_OPEN(
					"0", "empty", "no tests")
					HTML_TESTS_OPEN HTML_TESTS_CLOSE HTML_SUBMODULES_OPEN
						HTML_SUBMODULES_CLOSE HTML_SUMMARY_PCT("NaN%")
							HTML_MODULE_CLOSE HTML_DOC_CLOSE;
	ret = check_capture(c, exp);
	ct_module_destroy(empty);
	return (ret);
}

// submodules are nested sections and the recap is rendered in their parent
static int display_html_submodules(void)
{
	t_module   *root;
	t_module   *sub;
	t_capture  *c;
	int			ret;
	const char *exp;

	ct_module_init(&root, "root", "root description");
	sub = create_quiet_dummy();
	if (!sub)
		return (ct_module_destroy(root), EXIT_FAILURE);
	ct_module_add_sub(root, sub);
	ct_module_taddf(root, dummy_success_test, "r1");
	ct_module_run(root);
	c = ct_capture_create(STDOUT_FILENO);
	if (!c)
		return (ct_module_destroy(root), EXIT_FAILURE);
	ct_module_display(root, CT_FMT_HTML | CT_DT_FULLNAME | CT_DSBM_SUMPCT |
								CT_PRTY_NOCOLOR);
	exp = HTML_DOC_OPEN HTML_MODULE_OPEN("0", "root", "root description")
		HTML_TESTS_OPEN HTML_TEST_OK("root:r1")
			HTML_TESTS_CLOSE HTML_SUBMODULES_OPEN HTML_MODULE_OPEN(
				"1", "str", "tests for dummy string module")
				HTML_TESTS_OPEN HTML_TEST_OK("root:str:ss1")
					HTML_TESTS_CLOSE HTML_SUBMODULES_OPEN HTML_SUBMODULES_CLOSE
						HTML_SUMMARY_NB("1", "1")
							HTML_MODULE_CLOSE HTML_SUBMODULES_CLOSE
								HTML_SUMMARY_OPEN HTML_NB_LINE("2", "2")
									HTML_SBM_RECAP_PCT("str", "100.00%")
										HTML_SUMMARY_CLOSE HTML_MODULE_CLOSE
											HTML_DOC_CLOSE;
	ret = check_capture(c, exp);
	ct_module_destroy(root);
	return (ret);
}

// silent summaries only drop the footer, the document stays valid
static int display_html_silent(void)
{
	t_module   *str;
	t_capture  *c;
	int			ret;
	const char *exp;

	ct_module_init(&str, "str", "tests for dummy string module");
	ct_module_taddf(str, dummy_success_test, "ss1");
	ct_module_run(str);
	c = ct_capture_create(STDOUT_FILENO);
	if (!c)
		return (ct_module_destroy(str), EXIT_FAILURE);
	ct_module_display(str, CT_FMT_HTML | CT_DM_SUMSILENT | CT_PRTY_NOCOLOR);
	exp = HTML_DOC_OPEN HTML_MODULE_OPEN("0", "str",
										 "tests for dummy string module")
		HTML_TESTS_OPEN HTML_TEST_OK("ss1")
			HTML_TESTS_CLOSE HTML_SUBMODULES_OPEN HTML_SUBMODULES_CLOSE
				HTML_MODULE_CLOSE HTML_DOC_CLOSE;
	ret = check_capture(c, exp);
	ct_module_destroy(str);
	return (ret);
}

// markup in module metadata is escaped so the report stays well formed
static int display_html_escape(void)
{
	t_module   *esc;
	t_capture  *c;
	int			ret;
	const char *exp;

	ct_module_init(&esc, "a<b&c\"d'e", "<desc>");
	c = ct_capture_create(STDOUT_FILENO);
	if (!c)
		return (ct_module_destroy(esc), EXIT_FAILURE);
	ct_module_display(esc, CT_FMT_HTML | CT_PRTY_NOCOLOR);
	exp = HTML_DOC_OPEN HTML_MODULE_OPEN("0", "a&lt;b&amp;c&quot;d&#39;e",
										 "&lt;desc&gt;")
		HTML_TESTS_OPEN HTML_TESTS_CLOSE HTML_SUBMODULES_OPEN
			HTML_SUBMODULES_CLOSE HTML_SUMMARY_NB("0", "0")
				HTML_MODULE_CLOSE HTML_DOC_CLOSE;
	ret = check_capture(c, exp);
	ct_module_destroy(esc);
	return (ret);
}

// a multi module selection is wrapped in one single html document
static int display_html_selection(void)
{
	t_module   *one;
	t_module   *two;
	t_module   *selection[2];
	t_capture  *c;
	int			ret;
	const char *exp;

	ct_module_init(&one, "one", "first module");
	ct_module_init(&two, "two", "second module");
	if (!one || !two)
		return (ct_module_destroy(one), ct_module_destroy(two), EXIT_FAILURE);
	ct_module_taddf(one, dummy_success_test, "t1");
	ct_module_taddf(two, dummy_success_test, "t2");
	ct_module_run(one);
	ct_module_run(two);
	selection[0] = one;
	selection[1] = two;
	c = ct_capture_create(STDOUT_FILENO);
	if (!c)
		return (ct_module_destroy(one), ct_module_destroy(two), EXIT_FAILURE);
	cti_selection_display(selection, 2, CT_FMT_HTML | CT_PRTY_NOCOLOR);
	exp = HTML_DOC_OPEN HTML_MODULE_OPEN("0", "one", "first module")
		HTML_TESTS_OPEN HTML_TEST_OK("t1") HTML_TESTS_CLOSE HTML_SUBMODULES_OPEN
			HTML_SUBMODULES_CLOSE HTML_SUMMARY_NB("1", "1")
				HTML_MODULE_CLOSE HTML_MODULE_OPEN("0", "two", "second module")
					HTML_TESTS_OPEN HTML_TEST_OK("t2")
						HTML_TESTS_CLOSE HTML_SUBMODULES_OPEN
							HTML_SUBMODULES_CLOSE HTML_SUMMARY_NB("1", "1")
								HTML_MODULE_CLOSE HTML_DOC_CLOSE;
	ret = check_capture(c, exp);
	return (ct_module_destroy(one), ct_module_destroy(two), ret);
}

int ctit_module_display_html(void)
{
	int ret;

	ret = display_html_document();
	if (ret != EXIT_SUCCESS)
		return (ret + 10);
	ret = display_html_results();
	if (ret != EXIT_SUCCESS)
		return (ret + 20);
	ret = display_html_empty();
	if (ret != EXIT_SUCCESS)
		return (ret + 30);
	ret = display_html_submodules();
	if (ret != EXIT_SUCCESS)
		return (ret + 40);
	ret = display_html_silent();
	if (ret != EXIT_SUCCESS)
		return (ret + 50);
	ret = display_html_escape();
	if (ret != EXIT_SUCCESS)
		return (ret + 60);
	ret = display_html_selection();
	if (ret != EXIT_SUCCESS)
		return (ret + 70);
	return (ret);
}
