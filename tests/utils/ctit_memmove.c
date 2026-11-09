#include <stdio.h>
#include <stdlib.h>

#define CTESTS_INTERNAL_TESTS
#include "ct_utils.h"

int	ctit_memmove(void)
{
	char		str[10];
	const char	str3[] = "1234567a9";

	ct_memcpy(str, "123456789", 10);
	ct_memmove(str, "abc", 3);
	if (ct_memcmp(str, "abc456789", 10) != 0)
		return (1);
	ct_memmove(str, str3, 10);
	if (ct_memcmp(str, str3, 10) != 0)
		return (2);
	ct_memmove(str, str + 2, 5);
	if (ct_memcmp(str, "3456767a9", 10) != 0)
		return (3);
	ct_memmove(str + 2, str, 5);
	if (ct_memcmp(str, "3434567a9", 10) != 0)
		return (4);
	if (ct_memmove(str, str, 999) != str || ct_memmove(NULL, str, 99) != NULL
		|| ct_memmove(str, NULL, 99) != str || ct_memmove(str, str, 0) != str)
		return (5);
	return (EXIT_SUCCESS);
}
