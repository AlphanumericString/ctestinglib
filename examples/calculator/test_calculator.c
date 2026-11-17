#include <stdio.h>
#include <stdlib.h>

#include "calculator.h"
#include "ct_assert.h"
#include "ct_capture.h"
#include "ct_memory.h"
#include "ct_modules.h"

#define CALC_INPUT "1 + 2\n8 - 3\n6 * 7\n7 / 2\n4 / 0\nnope\n2 ^ 3\n"

#define CALC_STDOUT                                                            \
	"calc> 3\ncalc> 5\ncalc> 42\ncalc> 3\ncalc> calc> calc> calc> \n"

#define CALC_STDERR                                                            \
	"error: division by zero\n"                                                \
	"error: expected '<int> <op> <int>'\n"                                     \
	"error: expected '<int> <op> <int>'\n"

// A parametrized test receives one entry of this table: ct_module_taddf_param()
// stores the pointer and the test function reads it back from its argument.
typedef struct s_op_case
{
	const char *name;
	const char *token;
	t_calc_op	op;
} t_op_case;

static t_op_case g_op_cases[] = {
	{"plus", "+", CALC_ADD},
	{"minus", "-", CALC_SUB},
	{"times", "*", CALC_MUL},
	{"divide", "/", CALC_DIV},
};

typedef struct s_bad_token
{
	const char *name;
	const char *token;
} t_bad_token;

static t_bad_token g_bad_tokens[] = {
	{"empty", ""},
	{"double plus", "++"},
	{"unknown", "?"},
	{"digit", "1"},
};

static int check_op(const char *token, t_calc_op expected)
{
	t_calc_op op;

	return (ct_assert_true(
		calc_parse_op(token, &op) == 0 && op == expected,
		"calc_parse_op(\"%s\") returned the wrong operator\n", token));
}

static int check_op_error(const char *token)
{
	t_calc_op op;

	return (ct_assert_true(calc_parse_op(token, &op) != 0,
						   "calc_parse_op(\"%s\") should have failed\n",
						   token));
}

static int test_op_param(void *param)
{
	const t_op_case *test;

	test = param;
	if (!test)
		return (EXIT_FAILURE);
	return (check_op(test->token, test->op));
}

static int test_bad_token_param(void *param)
{
	const t_bad_token *test;

	test = param;
	if (!test)
		return (EXIT_FAILURE);
	return (check_op_error(test->token));
}

static int test_parse_guards(void)
{
	t_calc_op op;

	if (ct_assert_true(calc_parse_op(NULL, &op) != 0,
					   "calc_parse_op(NULL) should have failed\n") != 0)
		return (1);
	return (ct_assert_true(calc_parse_op("+", NULL) != 0,
						   "calc_parse_op(+, NULL) should have failed\n"));
}

static int check_apply(int lhs, t_calc_op op, int rhs, int expected)
{
	int value;

	value = 0xDEAD;
	return (ct_assert_true(calc_apply(lhs, op, rhs, &value) == 0 &&
							   value == expected,
						   "calc_apply(%d, %d, %d) returned %d, expected %d\n",
						   lhs, op, rhs, value, expected));
}

static int test_apply(void)
{
	int value;

	if (check_apply(40, CALC_ADD, 2, 42) != 0 ||
		check_apply(45, CALC_SUB, 3, 42) != 0 ||
		check_apply(21, CALC_MUL, 2, 42) != 0 ||
		check_apply(84, CALC_DIV, 2, 42) != 0)
		return (1);
	value = 42;
	if (ct_assert_true(calc_apply(1, CALC_DIV, 0, &value) == 2 && value == 42,
					   "calc_apply should report division by zero\n") != 0)
		return (2);
	if (ct_assert_true(calc_apply(1, (t_calc_op)42, 1, &value) != 0,
					   "calc_apply should reject an unknown operator\n") != 0)
		return (3);
	return (ct_assert_true(calc_apply(1, CALC_ADD, 1, NULL) != 0,
						   "calc_apply should reject a NULL result\n"));
}

static int check_eval(const char *line, int expected)
{
	int value;

	value = 0xDEAD;
	return (ct_assert_true(calc_eval(line, &value) == 0 && value == expected,
						   "calc_eval(\"%s\") returned %d, expected %d\n", line,
						   value, expected));
}

static int test_eval(void)
{
	int value;

	if (check_eval(" 12 / 5", 2) != 0 || check_eval("7 * 6", 42) != 0 ||
		check_eval("-4 + 10", 6) != 0)
		return (1);
	if (ct_assert_true(calc_eval("4 / 0", &value) == 2,
					   "calc_eval should report division by zero\n") != 0)
		return (2);
	if (ct_assert_true(calc_eval("4 +", &value) != 0 &&
						   calc_eval("nope", &value) != 0 &&
						   calc_eval("2 ^ 3", &value) != 0,
					   "calc_eval should reject malformed expressions\n") != 0)
		return (3);
	if (ct_assert_true(calc_eval(NULL, &value) != 0 &&
						   calc_eval("1 + 1", NULL) != 0,
					   "calc_eval should reject NULL arguments\n") != 0)
		return (4);
	return (ct_assert_true(calc_repl(NULL) != 0,
						   "calc_repl should reject a NULL input\n"));
}

// Module fixtures: setup prepares the history once, before the module's tests
// are forked, and teardown releases it after the last one. With FORK_TESTS each
// test starts from the empty history built by setup; the tests reset it anyway
// so they also keep working when the library is built without forks.
static t_calc_history g_history;

static void history_setup(void)
{
	calc_history_init(&g_history);
}

static void history_teardown(void)
{
	calc_history_destroy(&g_history);
}

static int test_history_push(void)
{
	// Clear the fixture so the test is independent of the run order and of
	// FORK_TESTS: setup only has to provide a valid history object.
	calc_history_destroy(&g_history);
	calc_history_init(&g_history);
	if (ct_assert_true(g_history.values == NULL && g_history.len == 0,
					   "a fresh history should be empty\n") != 0)
		return (1);
	if (ct_assert_true(calc_history_push(&g_history, 42) == 0 &&
						   calc_history_push(&g_history, 7) == 0 &&
						   g_history.len == 2 && g_history.values[0] == 42 &&
						   g_history.values[1] == 7,
					   "calc_history_push should append results\n") != 0)
		return (2);
	return (ct_assert_true(calc_history_push(&g_history, 3) == 0 &&
							   g_history.len == 3 && g_history.values[2] == 3,
						   "calc_history_push should keep the order\n"));
}

static int test_history_print(void)
{
	t_capture *out;
	char	  *text;
	int		   ret;

	calc_history_destroy(&g_history);
	calc_history_init(&g_history);
	if (calc_history_push(&g_history, 42) != 0 ||
		calc_history_push(&g_history, 7) != 0)
		return (1);
	out = ct_capture_create(STDOUT_FILENO);
	if (!out)
		return (2);
	calc_history_print(&g_history);
	text = ct_capture_get(out);
	ct_capture_destroy(out);
	if (!text)
		return (3);
	if (ct_assert_str_neq(text, "", "history should print its values\n") != 0)
		ret = 4;
	else if (ct_assert_str_eq(text, "42\n7\n", "history output was \"%s\"\n",
							  text) != 0)
		ret = 5;
	else
		ret = EXIT_SUCCESS;
	free(text);
	return (ret);
}

static int test_history_guards(void)
{
	if (ct_assert_false(calc_history_push(NULL, 1) == 0,
						"calc_history_push should reject NULL\n") != 0)
		return (1);
	calc_history_print(NULL);
	calc_history_destroy(NULL);
	return (EXIT_SUCCESS);
}

// Module fixtures can own any resource. The REPL module prepares two input
// files in setup and closes them in teardown, so the tests only have to run
// the REPL and compare what it printed.
static FILE *g_repl_input;
static FILE *g_repl_history_input;

static void repl_setup(void)
{
	g_repl_input = tmpfile();
	g_repl_history_input = tmpfile();
	if (g_repl_input)
	{
		fputs(CALC_INPUT, g_repl_input);
		rewind(g_repl_input);
	}
	if (g_repl_history_input)
	{
		fputs("1 + 2\nhistory\n", g_repl_history_input);
		rewind(g_repl_history_input);
	}
}

static void repl_teardown(void)
{
	if (g_repl_input)
		fclose(g_repl_input);
	if (g_repl_history_input)
		fclose(g_repl_history_input);
	g_repl_input = NULL;
	g_repl_history_input = NULL;
}

// The REPL writes its prompts and results on stdout and its errors on stderr.
// Two captures run at the same time so each descriptor can be checked on its
// own after the loop returns.
static int test_repl(void)
{
	t_capture *out;
	t_capture *err;
	int		   ret;

	if (!g_repl_input)
		return (1);
	out = ct_capture_create(STDOUT_FILENO);
	err = ct_capture_create(STDERR_FILENO);
	ret = EXIT_SUCCESS;
	if (!out || !err)
		ret = 2;
	else if (calc_repl(g_repl_input) != 0)
		ret = 3;
	else if (ct_capture_compare(out, CALC_STDOUT) != 0)
		ret = 4;
	else if (ct_capture_compare(err, CALC_STDERR) != 0)
		ret = 5;
	ct_capture_destroy(out);
	ct_capture_destroy(err);
	return (ret);
}

static int test_repl_history(void)
{
	t_capture *out;
	int		   ret;

	if (!g_repl_history_input)
		return (1);
	out = ct_capture_create(STDOUT_FILENO);
	if (!out)
		return (2);
	ret = calc_repl(g_repl_history_input);
	if (ret != 0 || ct_capture_compare(out, "calc> 3\ncalc> 3\ncalc> \n") != 0)
		ret = 3;
	else
		ret = EXIT_SUCCESS;
	ct_capture_destroy(out);
	return (ret);
}

static void auto_history(void)
{
	t_calc_history history;
	int			   i;

	calc_history_init(&history);
	i = 0;
	while (i < 40)
	{
		(void)calc_history_push(&history, i);
		i++;
	}
	calc_history_destroy(&history);
}

static FILE *g_auto_repl_input;

static void auto_repl(void)
{
	t_capture *out;

	if (!g_auto_repl_input)
		return;
	rewind(g_auto_repl_input);
	clearerr(g_auto_repl_input);
	out = ct_capture_create(STDOUT_FILENO);
	(void)calc_repl(g_auto_repl_input);
	// Consume before destroying: release flushes pending output to the
	// restored descriptor.
	(void)ct_capture_compare(out, "");
	ct_capture_destroy(out);
}

// ct_alloc lets a test count allocations, fail a chosen one, and sweep every
// failure point of a function. The history grows with malloc(), so it is a
// natural place to show all three.
static int test_alloc(void)
{
	t_calc_history history;
	t_capture	  *out;
	FILE		  *input;
	void		  *safe;
	void		  *forced;
	size_t		   before;
	size_t		   after;
	int			   ret;

	ct_failloc_reset();
	calc_history_init(&history);
	before = ct_alloc_getcount();
	if (ct_assert_true(calc_history_push(&history, 42) == 0 &&
						   history.len == 1 && history.values[0] == 42,
					   "the first push should allocate storage\n") != 0)
		return (calc_history_destroy(&history), 1);
	after = ct_alloc_getcount();
	if (ct_assert_true(after == before + 1,
					   "the first push should allocate exactly once\n") != 0)
		return (calc_history_destroy(&history), 2);
	before = ct_alloc_getcount();
	if (ct_assert_true(calc_history_push(&history, 7) == 0 &&
						   ct_alloc_getcount() == before,
					   "a push within the capacity should not allocate\n") != 0)
		return (calc_history_destroy(&history), 3);
	while (history.len < history.cap)
	{
		if (calc_history_push(&history, (int)history.len) != 0)
			return (calc_history_destroy(&history), 4);
	}
	before = ct_alloc_getcount();
	if (ct_assert_true(calc_history_push(&history, 99) == 0 &&
						   ct_alloc_getcount() == before + 1,
					   "growing the history should allocate once\n") != 0)
		return (calc_history_destroy(&history), 5);
	calc_history_destroy(&history);

	calc_history_init(&history);
	ct_failloc_set(0);
	ret = calc_history_push(&history, 1);
	ct_failloc_reset();
	if (ct_assert_true(ret != 0 && history.len == 0 && history.values == NULL,
					   "a failed first allocation should leave the history "
					   "empty\n") != 0)
		return (calc_history_destroy(&history), 6);
	if (calc_history_push(&history, 1) != 0)
		return (calc_history_destroy(&history), 7);
	while (history.len < history.cap)
	{
		if (calc_history_push(&history, (int)history.len) != 0)
			return (calc_history_destroy(&history), 8);
	}
	ct_failloc_set(0);
	ret = calc_history_push(&history, 99);
	ct_failloc_reset();
	if (ct_assert_true(ret != 0 && history.len == history.cap &&
						   history.values[0] == 1,
					   "a failed growth should preserve the history\n") != 0)
		return (calc_history_destroy(&history), 9);
	calc_history_destroy(&history);

	input = tmpfile();
	if (!input)
		return (10);
	fputs("1 + 1\n", input);
	rewind(input);
	out = ct_capture_create(STDOUT_FILENO);
	if (!out)
		return (fclose(input), 11);
	ct_failloc_set(0);
	ret = calc_repl(input);
	ct_failloc_reset();
	if (ct_assert_true(ret != 0,
					   "calc_repl should report a failed line allocation\n") !=
			0 ||
		ct_assert_true(ct_capture_compare(out, "") == 0,
					   "a failed line allocation should print nothing\n") != 0)
		return (ct_capture_destroy(out), fclose(input), 12);
	ct_capture_destroy(out);
	fclose(input);

	// safealloc()/safefree() are the escape hatch used by the library itself:
	// they are neither counted nor affected by failure injection.
	before = ct_alloc_getcount();
	safe = safealloc(16);
	after = ct_alloc_getcount();
	ct_failloc_set(0);
	forced = safealloc(16);
	ct_failloc_reset();
	if (ct_assert_true(safe != NULL && forced != NULL && after == before,
					   "safealloc should bypass counting and failloc\n") != 0)
		return (safefree(safe), safefree(forced), 13);
	safefree(safe);
	safefree(forced);

	ct_failloc_reset();
	ct_auto_call(auto_history);
	ct_failloc_reset();
	g_auto_repl_input = tmpfile();
	if (!g_auto_repl_input)
		return (14);
	fputs("1 + 1\n2 * 3\nhistory\n", g_auto_repl_input);
	rewind(g_auto_repl_input);
	ct_auto_call(auto_repl);
	ct_failloc_reset();
	fclose(g_auto_repl_input);
	g_auto_repl_input = NULL;
	return (EXIT_SUCCESS);
}

static t_module *create_operators_module(void)
{
	t_module *m;
	size_t	  i;

	ct_module_init(&m, "operators", "parse and apply one operator");
	if (!m)
		return (NULL);
	i = 0;
	while (i < sizeof(g_op_cases) / sizeof(g_op_cases[0]))
	{
		if (!ct_module_taddf_param(m, test_op_param, &g_op_cases[i],
								   g_op_cases[i].name))
			return (ct_module_destroy(m), NULL);
		i++;
	}
	i = 0;
	while (i < sizeof(g_bad_tokens) / sizeof(g_bad_tokens[0]))
	{
		if (!ct_module_taddf_param(m, test_bad_token_param, &g_bad_tokens[i],
								   g_bad_tokens[i].name))
			return (ct_module_destroy(m), NULL);
		i++;
	}
	if (!ct_module_taddf(m, test_parse_guards, "reject NULL arguments"))
		return (ct_module_destroy(m), NULL);
	if (!ct_module_taddf(m, test_apply, "apply operators"))
		return (ct_module_destroy(m), NULL);
	return (m);
}

static t_module *create_expressions_module(void)
{
	t_module *m;

	ct_module_init(&m, "expressions", "evaluate expressions");
	if (!m)
		return (NULL);
	if (!ct_module_taddf(m, test_eval, "evaluate expressions"))
		return (ct_module_destroy(m), NULL);
	return (m);
}

static t_module *create_history_module(void)
{
	t_module *m;

	ct_module_init(&m, "history", "dynamic result history");
	if (!m)
		return (NULL);
	ct_module_set_setup(m, history_setup);
	ct_module_set_teardown(m, history_teardown);
	if (!ct_module_taddf(m, test_history_push, "push results") ||
		!ct_module_taddf(m, test_history_print, "print results") ||
		!ct_module_taddf(m, test_history_guards, "reject NULL arguments"))
		return (ct_module_destroy(m), NULL);
	return (m);
}

static t_module *create_repl_module(void)
{
	t_module *m;

	ct_module_init(&m, "repl", "read-eval-print loop");
	if (!m)
		return (NULL);
	ct_module_set_setup(m, repl_setup);
	ct_module_set_teardown(m, repl_teardown);
	if (!ct_module_taddf(m, test_repl, "capture prompts, results and errors") ||
		!ct_module_taddf(m, test_repl_history, "history command"))
		return (ct_module_destroy(m), NULL);
	return (m);
}

static t_module *create_memory_module(void)
{
	t_module *m;

	ct_module_init(&m, "memory", "allocation failure showcase");
	if (!m)
		return (NULL);
	if (!ct_module_taddf(m, test_alloc, "ct_alloc showcase"))
		return (ct_module_destroy(m), NULL);
	return (m);
}

int main(int argc, char **argv)
{
	t_module *root;
	t_module *sub;
	t_module *(*creators[])(void) = {
		create_operators_module, create_expressions_module,
		create_history_module,	 create_repl_module,
		create_memory_module,
	};
	size_t i;
	int	   ret;

	ct_module_init(&root, "calculator", "tests for the calculator example");
	if (!root)
		return (EXIT_FAILURE);
	i = 0;
	while (i < sizeof(creators) / sizeof(creators[0]))
	{
		sub = creators[i]();
		i++;
		if (!sub || !ct_module_add_sub(root, sub))
			return (ct_module_destroy(root), EXIT_FAILURE);
	}
	ret = ct_module_run_args(root, argc, argv, CT_DT_OK);
	ct_module_destroy(root);
	return (ret);
}

#undef CALC_INPUT
#undef CALC_STDOUT
#undef CALC_STDERR
