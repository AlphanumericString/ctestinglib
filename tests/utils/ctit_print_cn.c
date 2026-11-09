#include <stdlib.h>
#include <unistd.h>

#include "ct_capture.h"

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_utils.h"

int	ctit_print_cn(void)
{
	t_capture	*cap;
	int			ret;

	cap = ct_capture_create(STDOUT_FILENO);
	cti_print_charn(5, '*');
	cti_print_charn(0, '*');
	cti_print_charn(-2, '*');
	cti_print_charn(99999, '\0');
	ret = EXIT_SUCCESS;
	if (ct_capture_compare(cap, "*****") != 0)
		ret = EXIT_FAILURE;
	ct_capture_destroy(cap);
	return (ret);
}
