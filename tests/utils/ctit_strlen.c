#include <stdlib.h>

#define CTESTS_INTERNAL_TESTS
#include "ct_utils.h"

int ctit_strlen(void)
{
	if (ct_strlen("") != 0)
		return (1);
	if (ct_strlen("Hello") != 5)
		return (2);
	if (ct_strlen("42 is the answer") != 16)
		return (3);
	if (ct_strlen("A longer string with multiple words.") != 36)
		return (4);
	return (EXIT_SUCCESS);
}
