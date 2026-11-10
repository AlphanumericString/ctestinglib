#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

#include "ct_capture.h"

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_utils.h"

int ctit_print_nb(void)
{
	t_capture *cap;

	cap = ct_capture_create(STDOUT_FILENO);
	cti_print_nb(0);
	cti_print_nb(42);
	cti_print_nb(99999);
	if (ct_capture_compare(cap, "04299999") != 0)
		return (ct_capture_destroy(cap), 1);
	cti_print_nb(INT_MAX);
	cti_print_nb(INT_MIN);
	cti_print_nb(INT_MIN + 2);
	if (ct_capture_compare(cap, "2147483647-2147483648-2147483646") != 0)
		return (ct_capture_destroy(cap), 2);
	return (ct_capture_destroy(cap), EXIT_SUCCESS);
}
