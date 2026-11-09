#include <signal.h>
#include <stdlib.h>

#define CTESTS_INTERNAL_TESTS
#include "ct_modules.h"

#include "ct_capture.h"

// smol populate
int	dummy_fail_test(void)
{
	return (EXIT_FAILURE);
}

int	dummy_success_test(void)
{
	return (EXIT_SUCCESS);
}

int	dummy_sigsegv(void)
{
	raise(SIGSEGV);
	return (EXIT_FAILURE);
}

int	dummy_capture_stdin(void)
{
	t_capture	*cap;
	int			ret;

	ret = EXIT_SUCCESS;
	cap = ct_capture_create(STDOUT_FILENO);
	if (!cap)
		return (EXIT_FAILURE);
	write(STDOUT_FILENO, "test", 4);
	if (ct_capture_compare(cap, "test") != 0)
		ret = EXIT_FAILURE;
	return (ct_capture_destroy(cap), ret);
}
