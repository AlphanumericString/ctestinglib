#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdlib.h>
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

// stdin is fed through a capture: ct_capture_stdin() redirects STDIN_FILENO to
// the input of the current case, ct_capture_destroy() restores the real stdin.
static int run_case(const char *input, const char *expected_out,
					const char *expected_err, int expected_ret)
{
	t_capture *in;
	t_capture *out;
	t_capture *err;
	int		   ret;

	in = ct_capture_stdin(input);
	out = ct_capture_create(STDOUT_FILENO);
	err = ct_capture_create(STDERR_FILENO);
	ret = EXIT_SUCCESS;
	if (!in || !out || !err)
		ret = 2;
	else if (fizzbuzz_run() != expected_ret)
		ret = 4;
	else if (ct_capture_compare(out, expected_out) != 0)
		ret = 5;
	else if (ct_capture_compare(err, expected_err) != 0)
		ret = 6;
	ct_capture_destroy(in);
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
	// The fixture is reset before every case, so the invalid input can not
	// leak into a following one: it is only kept last for readability.
	return (run_case("nope\n", PROMPT, ERRMSG, EXIT_FAILURE));
}

static void auto_fizzbuzz_run(void)
{
	t_capture *in;
	t_capture *out;
	t_capture *err;

	in = ct_capture_stdin("7\n");
	if (!in)
		return;
	out = ct_capture_create(STDOUT_FILENO);
	err = ct_capture_create(STDERR_FILENO);
	if (out && err)
		(void)fizzbuzz_run();
	// ct_capture_destroy() discards output still buffered in stdio, so the
	// captures can be released without consuming what fizzbuzz_run printed.
	ct_capture_destroy(in);
	ct_capture_destroy(out);
	ct_capture_destroy(err);
}

// fizzbuzz_run() allocates its state with malloc(). ct_alloc checks that the
// allocation is counted, that a failure is reported cleanly, and that every
// failure point of the run is exercised.
static int test_alloc(void)
{
	t_capture *in;
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

	in = ct_capture_stdin("7\n");
	out = ct_capture_create(STDOUT_FILENO);
	err = ct_capture_create(STDERR_FILENO);
	if (!in || !out || !err)
		return (ct_failloc_reset(), ct_capture_destroy(in),
				ct_capture_destroy(out), ct_capture_destroy(err), 3);
	ct_failloc_set(0);
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
	ct_capture_destroy(in);
	ct_capture_destroy(out);
	ct_capture_destroy(err);
	if (ret != EXIT_SUCCESS)
		return (ret);

	ct_auto_call(auto_fizzbuzz_run);
	ct_failloc_reset();
	return (EXIT_SUCCESS);
}

static t_module *create_response_module(void)
{
	t_module *m;

	ct_module_init(&m, "response", "fizz/buzz/bazz responses");
	if (!m)
		return (NULL);
	if (!ct_module_taddf(m, test_response, "response values"))
		return (ct_module_destroy(m), NULL);
	return (m);
}

static t_module *create_run_module(void)
{
	t_module *m;

	ct_module_init(&m, "run", "captured stdin and runs");
	if (!m)
		return (NULL);
	if (!ct_module_taddf(m, test_capture, "capture prompts and responses"))
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
		create_response_module,
		create_run_module,
		create_memory_module,
	};
	size_t i;
	int	   ret;

	ct_module_init(&root, "fizzbuzz", "tests for the fizzbuzz example");
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
