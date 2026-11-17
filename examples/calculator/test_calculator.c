#include <stdio.h>
#include <stdlib.h>

#include "calculator.h"
#include "ct_assert.h"
#include "ct_capture.h"
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
	ret = ct_module_run_args(root, argc, argv, CT_DT_OK);
	ct_module_destroy(root);
	return (ret);
}

#undef CALC_INPUT
#undef CALC_STDOUT
#undef CALC_STDERR
