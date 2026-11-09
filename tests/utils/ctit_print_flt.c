#include <stdlib.h>
#include <unistd.h>

#include "ct_capture.h"
#include "ct_memory.h"
#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_utils.h"

int ctit_print_flt(void)
{
	t_capture *cap;
	char	  *s;

	cap = ct_capture_create(STDOUT_FILENO);
	cti_print_flt(0.0f);
	cti_print_flt(42.42f);
	cti_print_flt(-42.42f);
	s = ct_capture_get(cap);
	ct_capture_destroy(cap);
	if (!s)
		return (1);
	if (ct_memcmp(s, "0.0042.42-42.42", 15) != 0)
		return (safefree(s), 2);
	safefree(s);
	return (EXIT_SUCCESS);
}
