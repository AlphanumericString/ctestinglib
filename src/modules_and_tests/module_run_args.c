#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define CTESTS_INTERNAL
#include "ct_memory.h"
#include "ct_modules.h"
#include "ct_utils.h"

#define RUN_MODULE_FLAG "--run-module="
#define RUN_MODULE_FLAG_LEN 13
#define RUN_MODULE_PREFIX "--run-module"
#define RUN_MODULE_PREFIX_LEN 12

#define HELP_FLAG_SHORT "-h"
#define HELP_FLAG_LONG "--help"

#define ERR_PRE "ctests: "
#define ERR_EMPTY_PATH ERR_PRE "empty module path\n"
#define ERR_EMPTY_SEG_PRE ERR_PRE "empty module name in path '"
#define ERR_NOT_FOUND_PRE ERR_PRE "module not found: '"
#define ERR_NOT_FOUND_POST "'\n"
#define ERR_IS_TEST_PRE ERR_PRE "test '"
#define ERR_IS_TEST_POST "' can not be run: only modules can be selected\n"
#define ERR_UNKNOWN_OPT_PRE ERR_PRE "unknown option: '"
#define ERR_EXPECT_VAL ERR_PRE "missing module path after '--run-module'\n"
#define ERR_TRY_HELP ERR_PRE "try '-h' or '--help' for usage\n"

// cli errors are written to stderr to keep stdout (the reports) parsable
static void loc_err_bytes(const char *str, size_t n)
{
	write(STDERR_FILENO, str, n);
}

static void loc_err_string(const char *str)
{
	loc_err_bytes(str, ct_strlen(str));
}

// usage printed on '-h' / '--help', the program name is prepended to the
// first line
static const char *const HELP_LINES[] = {
	" [OPTIONS] [MODULE...]\n",
	"\n",
	"Run the test modules selected on the command line.\n",
	"\n",
	"Options:\n",
	"  -h, --help          print this help and exit, nothing runs\n",
	"  --run-module=PATH   select the module at PATH\n",
	"\n",
	"Arguments:\n",
	"  MODULE              a module path to run, same as --run-module=PATH,\n"
	"                      both forms can be mixed and repeated\n",
	"\n",
	"A path is a ':' separated chain of module names resolved from the root\n"
	"module, the first name may be the root module itself. Selected modules\n"
	"run with their submodules and overlapping selections are deduplicated:\n"
	"the broadest module wins. Single tests can not be selected, only whole\n"
	"modules.\n",
	"\n",
	"When no module is given, the root module and its submodules are run.\n",
	NULL};

// the usage goes to stdout (stdout holds the reports while cli errors go
// to stderr) and argv[0] is used as the program name
static void loc_print_help(const char *prog)
{
	size_t i;

	if (!prog || *prog == '\0')
		prog = "tester";
	cti_print_string("Usage: ");
	cti_print_string(prog);
	i = 0;
	while (HELP_LINES[i] != NULL)
		cti_print_string(HELP_LINES[i++]);
}

// true when arg is exactly one of the help flags '-h' or '--help'
static bool loc_is_help(const char *arg)
{
	if (!arg)
		return (false);
	if (ct_strncmp(arg, HELP_FLAG_SHORT, 3) == 0)
		return (true);
	return (ct_strncmp(arg, HELP_FLAG_LONG, 7) == 0);
}

// scans argv for the help flags: help wins over any other argument, module
// selections and unknown options alike
static bool loc_args_help(int argc, char **argv)
{
	int i;

	i = 1;
	while (i < argc)
	{
		if (loc_is_help(argv[i]) == true)
			return (true);
		i++;
	}
	return (false);
}

// length of the segment between ':' separators
static size_t loc_seglen(const char *seg)
{
	size_t i;

	i = 0;
	while (seg[i] != '\0' && seg[i] != ':')
		i++;
	return (i);
}

// exact match of a ':' delimited segment (not null terminated) against name
static bool loc_name_eq(const char *name, const char *seg, size_t seglen)
{
	if (seglen == 0 || ct_strncmp(name, seg, seglen) != 0)
		return (false);
	return (name[seglen] == '\0');
}

static t_module *loc_find_sub(const t_module *m, const char *seg, size_t seglen)
{
	const t_list *sbm;

	sbm = m->submodules_list;
	while (sbm)
	{
		if (loc_name_eq(((const t_module *)sbm->data)->name, seg, seglen))
			return ((t_module *)sbm->data);
		sbm = sbm->next;
	}
	return (NULL);
}

static bool loc_is_test_name(const t_module *m, const char *seg, size_t seglen)
{
	const t_list *tls;

	tls = m->tests_list;
	while (tls)
	{
		if (loc_name_eq(((const t_test *)tls->data)->name, seg, seglen))
			return (true);
		tls = tls->next;
	}
	return (false);
}

static t_module *loc_resolve_err(const char *pre, const char *post,
								 const char *seg, size_t seglen,
								 const char *path)
{
	loc_err_string(pre);
	loc_err_bytes(seg, seglen);
	loc_err_string("' in path '");
	loc_err_string(path);
	loc_err_string(post);
	return (NULL);
}

static t_module *loc_resolve_empty(const char *path)
{
	loc_err_string(ERR_EMPTY_SEG_PRE);
	loc_err_string(path);
	loc_err_string("'\n");
	return (NULL);
}

// descends one segment into cur's submodules, NULL (+ printed error) on fail
static t_module *loc_descend(t_module *cur, const char *seg, size_t seglen,
							 const char *path)
{
	t_module *sub;

	sub = loc_find_sub(cur, seg, seglen);
	if (sub == NULL)
	{
		if (loc_is_test_name(cur, seg, seglen) == true)
			return (loc_resolve_err(ERR_IS_TEST_PRE, ERR_IS_TEST_POST, seg,
									seglen, path));
		return (loc_resolve_err(ERR_NOT_FOUND_PRE, ERR_NOT_FOUND_POST, seg,
								seglen, path));
	}
	return (sub);
}

// resolves a ':' separated module path from the cli against root
t_module *cti_module_resolve(t_module *root, const char *path)
{
	const char *seg;
	size_t		seglen;
	t_module   *cur;
	bool		first;

	if (!root || !path || *path == '\0')
		return (loc_err_string(ERR_EMPTY_PATH), NULL);
	cur = root;
	first = true;
	seg = path;
	while (*seg != '\0')
	{
		seglen = loc_seglen(seg);
		if (seglen == 0)
			return (loc_resolve_empty(path));
		if (first == false || loc_name_eq(cur->name, seg, seglen) == false)
			cur = loc_descend(cur, seg, seglen, path);
		if (cur == NULL)
			return (NULL);
		first = false;
		if (seg[seglen] == '\0')
			return (cur);
		seg += seglen + 1;
	}
	return (cur);
}

// true when candidate is a strict ancestor of module
static bool loc_is_ancestor(const t_module *candidate, const t_module *module)
{
	const t_module *parent;

	parent = module->parent;
	while (parent)
	{
		if (parent == candidate)
			return (true);
		parent = parent->parent;
	}
	return (false);
}

// drops every selection covered by a broader selection, and duplicates:
// each module runs at most once, the broadest selection wins
static int loc_dedupe(t_module **selection, int count)
{
	bool drop;
	int	 i;
	int	 j;
	int	 kept;

	kept = 0;
	i = 0;
	while (i < count)
	{
		drop = false;
		j = 0;
		while (j < count && drop == false)
		{
			if (j != i &&
				(loc_is_ancestor(selection[j], selection[i]) == true ||
				 (selection[j] == selection[i] && j < i)))
				drop = true;
			j++;
		}
		if (drop == false)
			selection[kept++] = selection[i];
		i++;
	}
	return (kept);
}

// maps one cli argument to a module: '--run-module=<path>' and a bare
// '<path>' both select modules, anything else starting with '-' is an error
static t_module *loc_arg_module(t_module *root, const char *arg)
{
	if (!arg)
		return (loc_err_string(ERR_EMPTY_PATH), NULL);
	if (ct_strncmp(arg, RUN_MODULE_FLAG, RUN_MODULE_FLAG_LEN) == 0)
		return (cti_module_resolve(root, arg + RUN_MODULE_FLAG_LEN));
	if (ct_strncmp(arg, RUN_MODULE_PREFIX, RUN_MODULE_PREFIX_LEN) == 0)
		return (loc_err_string(ERR_EXPECT_VAL), NULL);
	if (arg[0] == '-')
		return (loc_err_string(ERR_UNKNOWN_OPT_PRE), loc_err_string(arg),
				loc_err_string("'\n" ERR_TRY_HELP), NULL);
	return (cti_module_resolve(root, arg));
}

// resolves every cli argument against root. Returns the number of selected
// modules or -1: nothing is run when any argument is invalid (atomic check)
static int loc_parse_args(t_module *root, int argc, char **argv,
						  t_module **selection)
{
	t_module *m;
	int		  count;
	int		  i;

	count = 0;
	i = 1;
	while (i < argc)
	{
		m = loc_arg_module(root, argv[i]);
		if (m == NULL)
			return (-1);
		selection[count++] = m;
		i++;
	}
	return (count);
}

// no module given on the cli: run everything under root (default behavior)
static int loc_run_default(t_module *root, t_display_mode mode)
{
	int ret;

	ret = ct_module_run(root);
	ct_module_sort(root);
	ct_module_display(root, mode);
	return (ret);
}

// sorts then reports the selection: modules are reported one by one, json
// output wraps several modules in a single array to stay valid json
static void loc_report_selection(t_module **selection, int count,
								 t_display_mode mode)
{
	int i;

	i = 0;
	while (i < count)
	{
		ct_module_sort(selection[i]);
		i++;
	}
	if ((mode & CT_FMT_MSK) == CT_FMT_JSN && count > 1)
		return ((void)cti_display_results_json_list(selection, (size_t)count,
													mode));
	i = 0;
	while (i < count)
	{
		ct_module_display(selection[i], mode);
		i++;
	}
}

static int loc_run_selection(t_module **selection, int count,
							 t_display_mode mode)
{
	int ret;
	int i;

	ret = EXIT_SUCCESS;
	i = 0;
	while (i < count)
	{
		if (ct_module_run(selection[i]) != EXIT_SUCCESS)
			ret = EXIT_FAILURE;
		i++;
	}
	loc_report_selection(selection, count, mode);
	return (ret);
}

// runs the modules selected from the cli (see ct_modules.h for details),
// '-h' / '--help' prints the usage on stdout and returns success without
// running anything
int ct_module_run_args(t_module *root, int argc, char **argv,
					   t_display_mode mode)
{
	t_module **selection;
	int		   count;
	int		   ret;

	if (!root)
		return (EXIT_FAILURE);
	if (argc <= 1 || !argv)
		return (loc_run_default(root, mode));
	if (loc_args_help(argc, argv) == true)
		return (loc_print_help(argv[0]), EXIT_SUCCESS);
	selection = safealloc(sizeof(*selection) * (size_t)(argc - 1));
	if (!selection)
		return (EXIT_FAILURE);
	count = loc_parse_args(root, argc, argv, selection);
	if (count < 0)
		return (safefree(selection), EXIT_FAILURE);
	count = loc_dedupe(selection, count);
	ret = loc_run_selection(selection, count, mode);
	return (safefree(selection), ret);
}
