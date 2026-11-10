#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_capture.h"
#include "ct_memory.h"
#include "ct_modules.h"
#include "ct_utils.h"

#define RA_PROG "run_args"

#define RA_DISPLAY_EXP                                                         \
	"okmod\tmodule with passing tests\n"                                       \
	"\tnested\tnested module\n"                                                \
	"\t\tnest1  \033[32mOK\033[0m\n"                                           \
	"\tnested\t1 / 1\n"                                                        \
	"\tok1  \033[32mOK\033[0m\n"                                               \
	"okmod\t2 / 2\t[nested: 1 / 1]\n"

// root
// |- okmod   -> ok1 (passing), submodule nested -> nest1 (passing)
// |- komod   -> ko1 (failing)
static t_module *ra_tree(void)
{
	t_module *root;
	t_module *ok;
	t_module *ko;
	t_module *nest;

	ct_module_init(&root, "root", "root module");
	ct_module_init(&ok, "okmod", "module with passing tests");
	ct_module_init(&ko, "komod", "module with failing tests");
	ct_module_init(&nest, "nested", "nested module");
	if (!root || !ok || !ko || !nest)
		return (NULL);
	ct_module_taddf(ok, dummy_success_test, "ok1");
	ct_module_taddf(ko, dummy_fail_test, "ko1");
	ct_module_taddf(nest, dummy_success_test, "nest1");
	ct_module_add_sub(ok, nest);
	ct_module_add_sub(root, ok);
	ct_module_add_sub(root, ko);
	return (root);
}

// resolves the three reference modules of a freshly built tree
static bool ra_refs(t_module *root, t_module **ok, t_module **ko,
					t_module **nest)
{
	*ok = cti_module_resolve(root, "okmod");
	*ko = cti_module_resolve(root, "komod");
	*nest = cti_module_resolve(root, "okmod:nested");
	return (*ok != NULL && *ko != NULL && *nest != NULL);
}

// counts the occurrences of needle in hay
static size_t ra_count(const char *hay, const char *needle)
{
	size_t hits;
	size_t len;
	size_t i;

	hits = 0;
	len = ct_strlen(needle);
	i = 0;
	while (hay[i] != '\0')
	{
		if (ct_strncmp(hay + i, needle, len) == 0)
			hits++;
		i++;
	}
	return (hits);
}

static int ra_guards(void)
{
	char *argv[] = {RA_PROG, NULL};

	if (ct_module_run_args(NULL, 1, argv, CT_FMT_SILENT) != EXIT_FAILURE)
		return (1);
	if (ct_module_run_args(NULL, 0, NULL, CT_FMT_SILENT) != EXIT_FAILURE)
		return (2);
	return (EXIT_SUCCESS);
}

static int ra_resolve(void)
{
	t_module *root;
	t_module *m;
	t_module *n1;
	t_module *n2;

	root = ra_tree();
	if (!root)
		return (1);
	if (cti_module_resolve(root, "root") != root)
		return (ct_module_destroy(root), 2);
	n1 = cti_module_resolve(root, "okmod:nested");
	n2 = cti_module_resolve(root, "root:okmod:nested");
	m = cti_module_resolve(root, "okmod");
	if (!n1 || !n2 || n1 != n2 || !m || m->tests_count != 1 ||
		n2->tests_count != 1)
		return (ct_module_destroy(root), 3);
	ct_module_destroy(root);
	return (EXIT_SUCCESS);
}

static int ra_resolve_errors(void)
{
	t_capture *cap;
	t_module  *root;

	root = ra_tree();
	if (!root)
		return (1);
	cap = ct_capture_create(STDERR_FILENO);
	if (!cap)
		return (ct_module_destroy(root), 2);
	if (cti_module_resolve(root, "nope") != NULL)
		return (ct_capture_destroy(cap), ct_module_destroy(root), 3);
	if (cti_module_resolve(root, "okmod:ok1") != NULL)
		return (ct_capture_destroy(cap), ct_module_destroy(root), 4);
	if (cti_module_resolve(root, "") != NULL)
		return (ct_capture_destroy(cap), ct_module_destroy(root), 5);
	if (cti_module_resolve(root, "root::okmod") != NULL)
		return (ct_capture_destroy(cap), ct_module_destroy(root), 6);
	if (ct_capture_compare(cap,
						   "ctests: module not found: 'nope' in path "
						   "'nope'\n"
						   "ctests: test 'ok1' in path 'okmod:ok1' can not "
						   "be run: only modules can be selected\n"
						   "ctests: empty module path\n"
						   "ctests: empty module name in path "
						   "'root::okmod'\n") != 0)
		return (ct_capture_destroy(cap), ct_module_destroy(root), 7);
	ct_capture_destroy(cap);
	ct_module_destroy(root);
	return (EXIT_SUCCESS);
}

static int ra_run_flagged(void)
{
	char	 *argv[] = {RA_PROG, "--run-module=okmod", NULL};
	t_module *root;
	t_module *ok;
	t_module *ko;
	t_module *nest;

	root = ra_tree();
	if (!root || ra_refs(root, &ok, &ko, &nest) == false)
		return (ct_module_destroy(root), 1);
	if (ct_module_run_args(root, 2, argv, CT_FMT_SILENT) != EXIT_SUCCESS)
		return (ct_module_destroy(root), 2);
	if (ok->tests_passed != 1 || nest->tests_passed != 1 ||
		ok->module_passed != 1)
		return (ct_module_destroy(root), 3);
	if (ko->tests_passed != 0 || root->module_passed != 0)
		return (ct_module_destroy(root), 4);
	ct_module_destroy(root);
	return (EXIT_SUCCESS);
}

static int ra_run_positional(void)
{
	char	 *argv[] = {RA_PROG, "okmod", NULL};
	t_module *root;
	t_module *ok;
	t_module *ko;
	t_module *nest;

	root = ra_tree();
	if (!root || ra_refs(root, &ok, &ko, &nest) == false)
		return (ct_module_destroy(root), 1);
	if (ct_module_run_args(root, 2, argv, CT_FMT_SILENT) != EXIT_SUCCESS)
		return (ct_module_destroy(root), 2);
	if (ok->tests_passed != 1 || nest->tests_passed != 1)
		return (ct_module_destroy(root), 3);
	if (ko->tests_passed != 0 || root->module_passed != 0)
		return (ct_module_destroy(root), 4);
	ct_module_destroy(root);
	return (EXIT_SUCCESS);
}

// mixes the flagged and the positional forms: komod runs and fails, the
// run status follows ct_module_run semantics (failures live in counters)
static int ra_run_multi(void)
{
	char	 *argv[] = {RA_PROG, "--run-module=okmod", "komod", NULL};
	t_module *root;
	t_module *ok;
	t_module *ko;
	t_module *nest;

	root = ra_tree();
	if (!root || ra_refs(root, &ok, &ko, &nest) == false)
		return (ct_module_destroy(root), 1);
	if (ct_module_run_args(root, 3, argv, CT_FMT_SILENT) != EXIT_SUCCESS)
		return (ct_module_destroy(root), 2);
	if (ok->tests_passed != 1 || nest->tests_passed != 1)
		return (ct_module_destroy(root), 3);
	if (ko->tests_passed != 0 ||
		((t_test *)ko->tests_list->data)->return_value != EXIT_FAILURE)
		return (ct_module_destroy(root), 4);
	ct_module_destroy(root);
	return (EXIT_SUCCESS);
}

// a nested path runs the submodule alone, the parent own tests are skipped
static int ra_run_nested(void)
{
	char	 *argv[] = {RA_PROG, "okmod:nested", NULL};
	t_module *root;
	t_module *ok;
	t_module *ko;
	t_module *nest;

	root = ra_tree();
	if (!root || ra_refs(root, &ok, &ko, &nest) == false)
		return (ct_module_destroy(root), 1);
	if (ct_module_run_args(root, 2, argv, CT_FMT_SILENT) != EXIT_SUCCESS)
		return (ct_module_destroy(root), 2);
	if (nest->tests_passed != 1 || ok->tests_passed != 0 ||
		ok->module_passed != 0)
		return (ct_module_destroy(root), 3);
	ct_module_destroy(root);
	return (EXIT_SUCCESS);
}

// checks a full run (root and its submodules ran, komod failed)
static int ra_check_full(t_module *root)
{
	t_module *ok;
	t_module *ko;
	t_module *nest;

	if (ra_refs(root, &ok, &ko, &nest) == false)
		return (1);
	if (ok->tests_passed != 1 || nest->tests_passed != 1 ||
		ko->tests_passed != 0 ||
		((t_test *)ko->tests_list->data)->return_value != EXIT_FAILURE)
		return (2);
	// both submodules "return success" (fork semantics), proving root ran
	if (root->module_passed != 2)
		return (3);
	return (EXIT_SUCCESS);
}

// selecting root by name runs the whole tree
static int ra_run_root_name(void)
{
	char	 *argv[] = {RA_PROG, "root", NULL};
	t_module *root;
	int		  ret;

	root = ra_tree();
	if (!root)
		return (1);
	if (ct_module_run_args(root, 2, argv, CT_FMT_SILENT) != EXIT_SUCCESS)
		return (ct_module_destroy(root), 2);
	ret = ra_check_full(root);
	return (ct_module_destroy(root), ret == 0 ? EXIT_SUCCESS : 10 + ret);
}

// no module given: default behavior, root and its submodules run
static int ra_run_default(void)
{
	char	 *argv[] = {RA_PROG, NULL};
	t_module *root;
	int		  ret;

	root = ra_tree();
	if (!root)
		return (1);
	if (ct_module_run_args(root, 1, argv, CT_FMT_SILENT) != EXIT_SUCCESS)
		return (ct_module_destroy(root), 2);
	ret = ra_check_full(root);
	ct_module_destroy(root);
	if (ret != EXIT_SUCCESS)
		return (10 + ret);
	root = ra_tree();
	if (!root)
		return (3);
	if (ct_module_run_args(root, 0, NULL, CT_FMT_SILENT) != EXIT_SUCCESS)
		return (ct_module_destroy(root), 4);
	ret = ra_check_full(root);
	return (ct_module_destroy(root), ret == 0 ? EXIT_SUCCESS : 20 + ret);
}

// duplicates and overlaps run only once: the broadest selection wins
static int ra_run_dedupe(void)
{
	char	 *dup[] = {RA_PROG, "okmod", "okmod", NULL};
	char	 *shadow[] = {RA_PROG, "okmod:nested", "okmod", NULL};
	char	 *broad[] = {RA_PROG, "okmod", "root", NULL};
	t_module *root;
	t_module *ok;
	t_module *ko;
	t_module *nest;

	root = ra_tree();
	if (!root || ra_refs(root, &ok, &ko, &nest) == false)
		return (ct_module_destroy(root), 1);
	if (ct_module_run_args(root, 3, dup, CT_FMT_SILENT) != EXIT_SUCCESS)
		return (ct_module_destroy(root), 2);
	if (ok->tests_passed != 1 || nest->tests_passed != 1)
		return (ct_module_destroy(root), 3);
	ct_module_destroy(root);
	root = ra_tree();
	if (!root || ra_refs(root, &ok, &ko, &nest) == false)
		return (ct_module_destroy(root), 4);
	if (ct_module_run_args(root, 3, shadow, CT_FMT_SILENT) != EXIT_SUCCESS)
		return (ct_module_destroy(root), 5);
	if (nest->tests_passed != 1 || ok->tests_passed != 1)
		return (ct_module_destroy(root), 6);
	ct_module_destroy(root);
	root = ra_tree();
	if (!root || ra_refs(root, &ok, &ko, &nest) == false)
		return (ct_module_destroy(root), 7);
	if (ct_module_run_args(root, 3, broad, CT_FMT_SILENT) != EXIT_SUCCESS)
		return (ct_module_destroy(root), 8);
	if (ok->tests_passed != 1 || nest->tests_passed != 1 ||
		root->module_passed != 2)
		return (ct_module_destroy(root), 9);
	return (ct_module_destroy(root), EXIT_SUCCESS);
}

// invalid cli options stop everything before any test runs (atomic check)
static int ra_cli_errors(void)
{
	char	  *unknown[] = {RA_PROG, "--nope", NULL};
	char	  *noval[] = {RA_PROG, "--run-module", NULL};
	char	  *late[] = {RA_PROG, "--run-module=okmod", "--bad", NULL};
	t_capture *cap;
	t_module  *root;
	t_module  *ok;
	t_module  *ko;
	t_module  *nest;

	root = ra_tree();
	if (!root || ra_refs(root, &ok, &ko, &nest) == false)
		return (ct_module_destroy(root), 1);
	cap = ct_capture_create(STDERR_FILENO);
	if (!cap)
		return (ct_module_destroy(root), 2);
	if (ct_module_run_args(root, 2, unknown, CT_FMT_SILENT) != EXIT_FAILURE)
		return (ct_capture_destroy(cap), ct_module_destroy(root), 3);
	if (ct_module_run_args(root, 2, noval, CT_FMT_SILENT) != EXIT_FAILURE)
		return (ct_capture_destroy(cap), ct_module_destroy(root), 4);
	if (ct_module_run_args(root, 3, late, CT_FMT_SILENT) != EXIT_FAILURE)
		return (ct_capture_destroy(cap), ct_module_destroy(root), 5);
	if (ok->tests_passed != 0 || nest->tests_passed != 0)
		return (ct_capture_destroy(cap), ct_module_destroy(root), 6);
	if (ct_capture_compare(cap, "ctests: unknown option: '--nope'\n"
								"ctests: try '-h' or '--help' for usage\n"
								"ctests: missing module path after "
								"'--run-module'\n"
								"ctests: unknown option: '--bad'\n"
								"ctests: try '-h' or '--help' for usage\n") != 0)
		return (ct_capture_destroy(cap), ct_module_destroy(root), 7);
	ct_capture_destroy(cap);
	ct_module_destroy(root);
	return (EXIT_SUCCESS);
}

// -h and --help print the usage on stdout and run nothing: help wins over
// module selections, the exit status is success whatever the display mode
static int ra_help(void)
{
	char	  *short_h[] = {RA_PROG, "-h", NULL};
	char	  *long_h[] = {RA_PROG, "--help", NULL};
	char	  *mixed[] = {RA_PROG, "okmod", "-h", NULL};
	t_capture *cap;
	t_module  *root;
	t_module  *ok;
	t_module  *ko;
	t_module  *nest;
	char	  *out;

	root = ra_tree();
	if (!root || ra_refs(root, &ok, &ko, &nest) == false)
		return (ct_module_destroy(root), 1);
	cap = ct_capture_create(STDOUT_FILENO);
	if (!cap)
		return (ct_module_destroy(root), 2);
	if (ct_module_run_args(root, 2, short_h, CT_FMT_SILENT) != EXIT_SUCCESS)
		return (ct_capture_destroy(cap), ct_module_destroy(root), 3);
	if (ct_module_run_args(root, 2, long_h, CT_FMT_SILENT) != EXIT_SUCCESS)
		return (ct_capture_destroy(cap), ct_module_destroy(root), 4);
	if (ct_module_run_args(root, 3, mixed, CT_FMT_SILENT) != EXIT_SUCCESS)
		return (ct_capture_destroy(cap), ct_module_destroy(root), 5);
	if (ok->tests_passed != 0 || nest->tests_passed != 0)
		return (ct_capture_destroy(cap), ct_module_destroy(root), 6);
	out = ct_capture_get(cap);
	if (!out || ra_count(out, "Usage: " RA_PROG) != 3 ||
		ra_count(out, "-h, --help") != 3 ||
		ra_count(out, "--run-module=") != 6)
		return (safefree(out), ct_capture_destroy(cap),
				ct_module_destroy(root), 7);
	return (safefree(out), ct_capture_destroy(cap), ct_module_destroy(root),
			EXIT_SUCCESS);
}

// the selection is reported: only the selected module and its submodules
static int ra_display(void)
{
	char	  *argv[] = {RA_PROG, "okmod", NULL};
	t_capture *cap;
	t_module  *root;
	char	  *out;
	int		   ret;

	root = ra_tree();
	if (!root)
		return (1);
	cap = ct_capture_create(STDOUT_FILENO);
	if (!cap)
		return (ct_module_destroy(root), 2);
	ret = ct_module_run_args(root, 2, argv, CT_DT_OK | CT_PRTY_NOCOLOR);
	out = ct_capture_get(cap);
	if (ret != EXIT_SUCCESS || !out ||
		ct_strncmp(out, RA_DISPLAY_EXP, ct_strlen(RA_DISPLAY_EXP) + 1) != 0)
		return (safefree(out), ct_capture_destroy(cap), ct_module_destroy(root),
				3);
	return (safefree(out), ct_capture_destroy(cap), ct_module_destroy(root),
			EXIT_SUCCESS);
}

// several modules in json mode are wrapped in a single valid json array
static int ra_display_json(void)
{
	char	  *argv[] = {RA_PROG, "--run-module=okmod", "komod", NULL};
	t_capture *cap;
	t_module  *root;
	char	  *out;
	size_t	   len;

	root = ra_tree();
	if (!root)
		return (1);
	cap = ct_capture_create(STDOUT_FILENO);
	if (!cap)
		return (ct_module_destroy(root), 2);
	if (ct_module_run_args(root, 3, argv, CT_FMT_JSN) != EXIT_SUCCESS)
		return (ct_capture_destroy(cap), ct_module_destroy(root), 3);
	out = ct_capture_get(cap);
	if (!out)
		return (ct_capture_destroy(cap), ct_module_destroy(root), 4);
	len = ct_strlen(out);
	if (len < 4 || out[0] != '[' || out[1] != '{' || out[len - 3] != '}' ||
		out[len - 2] != ']' || out[len - 1] != '\n' ||
		ra_count(out, "\"module_name\"") != 3)
		return (safefree(out), ct_capture_destroy(cap), ct_module_destroy(root),
				5);
	return (safefree(out), ct_capture_destroy(cap), ct_module_destroy(root),
			EXIT_SUCCESS);
}

int ctit_run_args(void)
{
	int ret;

	ret = ra_guards();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = ra_resolve();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = ra_resolve_errors();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = ra_run_flagged();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = ra_run_positional();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = ra_run_multi();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = ra_run_nested();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = ra_run_root_name();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = ra_run_default();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = ra_run_dedupe();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = ra_cli_errors();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = ra_help();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = ra_display();
	if (ret != EXIT_SUCCESS)
		return (ret);
	return (ra_display_json());
}
