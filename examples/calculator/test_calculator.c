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

static int check_apply(int lhs, t_calc_op op, int rhs, int expected)
{
	int value;

	value = 0xDEAD;
	return (ct_assert_true(calc_apply(lhs, op, rhs, &value) == 0 &&
							   value == expected,
						   "calc_apply(%d, %d, %d) returned %d, expected %d\n",
						   lhs, op, rhs, value, expected));
}

static int check_eval(const char *line, int expected)
{
	int value;

	value = 0xDEAD;
	return (ct_assert_true(calc_eval(line, &value) == 0 && value == expected,
						   "calc_eval(\"%s\") returned %d, expected %d\n", line,
						   value, expected));
}

static int test_parse(void)
{
	t_calc_op op;

	if (check_op("+", CALC_ADD) != 0 || check_op("-", CALC_SUB) != 0 ||
		check_op("*", CALC_MUL) != 0 || check_op("/", CALC_DIV) != 0)
		return (1);
	if (check_op_error("") != 0 || check_op_error("++") != 0 ||
		check_op_error("?") != 0 || check_op_error("1") != 0)
		return (2);
	if (ct_assert_true(calc_parse_op(NULL, &op) != 0,
					   "calc_parse_op(NULL) should have failed\n") != 0)
		return (3);
	return (ct_assert_true(calc_parse_op("+", NULL) != 0,
						   "calc_parse_op(+, NULL) should have failed\n"));
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

// The REPL writes its prompts and results on stdout and its errors on stderr.
// Two captures run at the same time so each descriptor can be checked on its
// own after the loop returns.
static int test_repl(void)
{
	FILE	  *input;
	t_capture *out;
	t_capture *err;
	int		   ret;

	input = tmpfile();
	if (!input)
		return (1);
	fputs(CALC_INPUT, input);
	rewind(input);
	out = ct_capture_create(STDOUT_FILENO);
	err = ct_capture_create(STDERR_FILENO);
	ret = EXIT_SUCCESS;
	if (!out || !err)
		ret = 2;
	else if (calc_repl(input) != 0)
		ret = 3;
	else if (ct_capture_compare(out, CALC_STDOUT) != 0)
		ret = 4;
	else if (ct_capture_compare(err, CALC_STDERR) != 0)
		ret = 5;
	ct_capture_destroy(out);
	ct_capture_destroy(err);
	fclose(input);
	return (ret);
}

static int test_history(void)
{
	t_calc_history history;
	t_capture	  *out;
	FILE		  *input;
	int			   ret;

	calc_history_init(&history);
	if (ct_assert_true(calc_history_push(&history, 42) == 0 &&
						   calc_history_push(&history, 7) == 0 &&
						   history.len == 2 && history.values[0] == 42 &&
						   history.values[1] == 7,
					   "calc_history_push should append results\n") != 0)
		return (calc_history_destroy(&history), 1);
	out = ct_capture_create(STDOUT_FILENO);
	if (!out)
		return (calc_history_destroy(&history), 2);
	calc_history_print(&history);
	ret = ct_capture_compare(out, "42\n7\n");
	ct_capture_destroy(out);
	calc_history_destroy(&history);
	if (ret != 0)
		return (3);
	input = tmpfile();
	if (!input)
		return (4);
	fputs("1 + 2\nhistory\n", input);
	rewind(input);
	out = ct_capture_create(STDOUT_FILENO);
	if (!out)
		return (fclose(input), 5);
	ret = calc_repl(input);
	if (ret != 0 || ct_capture_compare(out, "calc> 3\ncalc> 3\ncalc> \n") != 0)
		ret = 6;
	else
		ret = EXIT_SUCCESS;
	ct_capture_destroy(out);
	fclose(input);
	if (ct_assert_true(calc_history_push(NULL, 1) != 0,
					   "calc_history_push should reject NULL\n") != 0)
		return (7);
	calc_history_print(NULL);
	calc_history_destroy(NULL);
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

static FILE *g_repl_input;

static void auto_repl(void)
{
	t_capture *out;

	if (!g_repl_input)
		return;
	rewind(g_repl_input);
	clearerr(g_repl_input);
	out = ct_capture_create(STDOUT_FILENO);
	(void)calc_repl(g_repl_input);
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
	g_repl_input = tmpfile();
	if (!g_repl_input)
		return (14);
	fputs("1 + 1\n2 * 3\nhistory\n", g_repl_input);
	rewind(g_repl_input);
	ct_auto_call(auto_repl);
	ct_failloc_reset();
	fclose(g_repl_input);
	g_repl_input = NULL;
	return (EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
	t_module *root;
	int		  ret;

	ct_module_init(&root, "calculator", "tests for the calculator example");
	if (!root)
		return (EXIT_FAILURE);
	ct_module_taddf(root, test_parse, "parse operators");
	ct_module_taddf(root, test_apply, "apply operators");
	ct_module_taddf(root, test_eval, "evaluate expressions");
	ct_module_taddf(root, test_repl, "capture repl output");
	ct_module_taddf(root, test_history, "dynamic result history");
	ct_module_taddf(root, test_alloc, "ct_alloc showcase");
	ret = ct_module_run_args(root, argc, argv, CT_DT_OK);
	ct_module_destroy(root);
	return (ret);
}

#undef CALC_INPUT
#undef CALC_STDOUT
#undef CALC_STDERR
