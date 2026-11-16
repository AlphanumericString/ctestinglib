#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_internal.h"

#define TEST_STRING "this\nis\va\rtEst\"tsT\'on_@prINTing!STRINgs"

int ctit_print_string(void)
{
	t_capture *cap;
	char	  *s;

	cap = ct_capture_create(STDOUT_FILENO);
	cti_print_string(TEST_STRING);
	s = ct_capture_get(cap);
	ct_capture_destroy(cap);
	if (!s)
		return (1);
	if (strcmp(s, TEST_STRING) != 0)
		return (safefree(s), 2);
	return (safefree(s), EXIT_SUCCESS);
}

#undef TEST_STRING
