#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_utils.h"

int ctit_cmpalpha(void)
{
	if (cti_cmp_alpha("Apple", "Apple") != 0)
		return (1);
	if (cti_cmp_alpha("Apple", "Applf") >= 0)
		return (2);
	if (cti_cmp_alpha("Applf", "Apple") <= 0)
		return (3);
	if (cti_cmp_alpha(NULL, "Apple") >= 0 || cti_cmp_alpha("Apple", NULL) <= 0)
		return (4);
	if (cti_cmp_alpha(NULL, NULL) != 0)
		return (5);
	return (EXIT_SUCCESS);
}
