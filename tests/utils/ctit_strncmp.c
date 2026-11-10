#include <stdlib.h>

#define CTESTS_INTERNAL_TESTS
#include "ct_utils.h"

int ctit_strncmp(void)
{
	if (ct_strncmp("Hello", "Hello", 5) != 0 ||
		ct_strncmp("Hello", "Hell", 4) != 0)
		return (1);
	if (ct_strncmp("Hello", "Hell", 5) <= 0)
		return (2);
	if (ct_strncmp("Apple", "Banana", 3) >= 0)
		return (3);
	if (ct_strncmp("Short", "Shorter", 10) >= 0)
		return (4);
	if (ct_strncmp("Same", "Same", 10) != 0)
		return (5);
	return (EXIT_SUCCESS);
}
