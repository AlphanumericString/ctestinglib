#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ct_assert.h"
#include "ct_capture.h"
#include "ct_memory.h"
#include "ct_modules.h"
#include "fizzbuzz.h"

static int test_response(void)
{
	t_fizzbuzz fb;
	t_capture *out;
	int		   ret;

	if (ct_assert_true(fizzbuzz_scan(NULL) != 0,
					   "fizzbuzz_scan should reject a NULL value\n") != 0)
		return (1);
	fb.value = 15;
	out = ct_capture_create(STDOUT_FILENO);
	if (!out)
		return (2);
	fizzbuzz_response(NULL);
	fizzbuzz_response(&fb);
	ret = ct_capture_compare(out, "bazz\n");
	ct_capture_destroy(out);
	return (ret == 0 ? EXIT_SUCCESS : 3);
}

// ct_capture only captures output descriptors. Feeding stdin is therefore done
// by hand: write the input in a pipe and dup2() its read end on STDIN_FILENO.
static int feed_stdin(const char *input)
{
	int		fds[2];
	size_t	len;
	ssize_t written;

	len = strlen(input);
	if (pipe(fds) == -1)
		return (1);
	if (len != 0)
	{
		written = write(fds[1], input, len);
		if (written != (ssize_t)len)
		{
			close(fds[0]);
			close(fds[1]);
			return (1);
		}
	}
	close(fds[1]);
	if (dup2(fds[0], STDIN_FILENO) == -1)
	{
		close(fds[0]);
		return (1);
	}
	close(fds[0]);
	// the previous run may have reached EOF: reset the stdio error state.
	clearerr(stdin);
	return (0);
}

static int run_case(const char *input, const char *expected_out,
					const char *expected_err, int expected_ret)
{
	t_capture *out;
	t_capture *err;
	int		   saved_stdin;
	int		   ret;

	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin == -1)
		return (1);
	out = ct_capture_create(STDOUT_FILENO);
	err = ct_capture_create(STDERR_FILENO);
	ret = EXIT_SUCCESS;
	if (!out || !err)
		ret = 2;
	else if (feed_stdin(input) != 0)
		ret = 3;
	else if (fizzbuzz_run() != expected_ret)
		ret = 4;
	else if (ct_capture_compare(out, expected_out) != 0)
		ret = 5;
	else if (ct_capture_compare(err, expected_err) != 0)
		ret = 6;
	if (dup2(saved_stdin, STDIN_FILENO) == -1 && ret == EXIT_SUCCESS)
		ret = 7;
	close(saved_stdin);
	clearerr(stdin);
	ct_capture_destroy(out);
	ct_capture_destroy(err);
	return (ret);
}

static int test_capture(void)
{
	static const struct
	{
		const char *input;
		const char *out;
	} cases[] = {
		{"15\n", PROMPT "bazz\n"}, {"9\n", PROMPT "fizz\n"},
		{"10\n", PROMPT "buzz\n"}, {"7\n", PROMPT "7\n"},
		{"0\n", PROMPT "bazz\n"},  {"-5\n", PROMPT "buzz\n"},
	};
	size_t i;

	i = 0;
	while (i < sizeof(cases) / sizeof(cases[0]))
	{
		if (run_case(cases[i].input, cases[i].out, "", EXIT_SUCCESS) != 0)
			return (1);
		i++;
	}
	// invalid input is checked last: scanf() keeps the offending character in
	// the stdin stream, which would leak into the following case.
	return (run_case("nope\n", PROMPT, ERRMSG, EXIT_FAILURE));
}

static void auto_fizzbuzz_run(void)
{
	t_capture *out;
	t_capture *err;

	if (feed_stdin("7\n") != 0)
		return;
	out = ct_capture_create(STDOUT_FILENO);
	err = ct_capture_create(STDERR_FILENO);
	(void)fizzbuzz_run();
	// Consume the output before destroying the captures: release flushes
	// pending stdio buffers to the restored descriptor.
	(void)ct_capture_compare(out, "");
	(void)ct_capture_compare(err, "");
	ct_capture_destroy(out);
	ct_capture_destroy(err);
}

// fizzbuzz_run() allocates its state with malloc(). ct_alloc checks that the
// allocation is counted, that a failure is reported cleanly, and that every
// failure point of the run is exercised.
static int test_alloc(void)
{
	t_capture *out;
	t_capture *err;
	size_t	   before;
	size_t	   after;
	int		   ret;

	ct_failloc_reset();
	auto_fizzbuzz_run();
	before = ct_alloc_getcount();
	auto_fizzbuzz_run();
	after = ct_alloc_getcount();
	if (ct_assert_true(after == before + 1,
					   "fizzbuzz_run should allocate its state once\n") != 0)
		return (1);

	ct_failloc_set(0);
	if (feed_stdin("7\n") != 0)
		return (ct_failloc_reset(), 2);
	out = ct_capture_create(STDOUT_FILENO);
	err = ct_capture_create(STDERR_FILENO);
	if (!out || !err)
		return (ct_failloc_reset(), ct_capture_destroy(out),
				ct_capture_destroy(err), 3);
	ret = fizzbuzz_run();
	ct_failloc_reset();
	if (ret != EXIT_FAILURE)
		ret = 4;
	else if (ct_capture_compare(out, "") != 0)
		ret = 5;
	else if (ct_capture_compare(err, ERRMSG_ALLOC) != 0)
		ret = 6;
	else
		ret = EXIT_SUCCESS;
	ct_capture_destroy(out);
	ct_capture_destroy(err);
	if (ret != EXIT_SUCCESS)
		return (ret);

	ct_auto_call(auto_fizzbuzz_run);
	ct_failloc_reset();
	return (EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
	t_module *root;
	int		  ret;

	ct_module_init(&root, "fizzbuzz", "tests for the fizzbuzz example");
	if (!root)
		return (EXIT_FAILURE);
	ct_module_taddf(root, test_response, "response values");
	ct_module_taddf(root, test_capture, "capture prompts and responses");
	ct_module_taddf(root, test_alloc, "ct_alloc showcase");
	ret = ct_module_run_args(root, argc, argv, CT_DT_OK);
	ct_module_destroy(root);
	return (ret);
}
