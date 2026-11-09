#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

#include "ct_capture.h"
#include "ct_memory.h"

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_utils.h"

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
	if (ct_memcmp(s, TEST_STRING, ct_strlen(TEST_STRING)) != 0)
		return (safefree(s), 2);
	return (safefree(s), EXIT_SUCCESS);
}

#undef TEST_STRING
