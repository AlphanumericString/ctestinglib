#include <stdlib.h>

#define CTESTS_INTERNAL_TESTS
#include "ct_utils.h"

int	ctit_memset(void)
{
	char	str[10];
	char	str2[10];

	ct_memcpy(str, "123456789", 10);
	ct_memcpy(str2, "123456789", 10);
	ct_memset(str, 'a', 10);
	if (ct_memcmp(str, "aaaaaaaaaa", 10) != 0)
		return (1);
	ct_memset(str, 'b', 0);
	if (ct_memcmp(str, "aaaaaaaaaa", 10) != 0)
		return (2);
	ct_memset(str, 'c', 5);
	if (ct_memcmp(str, "cccccaaaaa", 10) != 0)
		return (3);
	return (EXIT_SUCCESS);
}
