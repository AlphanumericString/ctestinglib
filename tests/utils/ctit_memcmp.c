#include <stdlib.h>

#define CTESTS_INTERNAL_TESTS
#include "ct_utils.h"

int	ctit_memcmp(void)
{
	const char	str_digit1[] = "1234567\0009";
	const char	str_digit2[] = "1234567\0009";
	const char	str_char[] = "1234567\000a";

	if (ct_memcmp(str_digit1, str_digit2, 10) != 0)
		return (1);
	if (ct_memcmp(str_digit1, str_char, 10) >= 0)
		return (2);
	if (ct_memcmp(str_char, str_digit1, 10) <= 0)
		return (3);
	if (ct_memcmp(str_digit1, str_digit2, 0) != 0
		|| ct_memcmp(str_digit1, str_digit1, 999) != 0)
		return (4);
	if (ct_memcmp(NULL, str_digit2, 12) >= 0
		|| ct_memcmp(str_digit1, NULL, 12) <= 0)
		return (5);
	return (EXIT_SUCCESS);
}
