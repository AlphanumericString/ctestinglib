#include <stdlib.h>
#include <sys/wait.h>

#define CTESTS_INTERNAL
#include "ct_internal.h"

bool cti_test_succeeded(const t_test *t)
{
	const int success_code = EXIT_SUCCESS;

	return (WIFEXITED(t->return_value) &&
			WEXITSTATUS(t->return_value) == success_code);
}
