#include <stdlib.h>

#define CTESTS_INTERNAL_TESTS
#include "ct_utils.h"

int	ctit_memcpy(void)
{
	char		str_buffer[10];
	const char	str_digits[] = "1234567\0009";

	if (ct_memcpy(str_buffer, "123456789", 10) != str_buffer
		|| ct_memcpy(str_buffer, "abc", 3) != str_buffer)
		return (1);
	if (ct_memcmp(str_buffer, "abc456789", 10) != 0)
		return (2);
	ct_memcpy(str_buffer, str_digits, 10);
	if (ct_memcmp(str_buffer, str_digits, 10) != 0)
		return (3);
	ct_memcpy(str_buffer, str_buffer, 999);
	if (ct_memcpy(NULL, str_digits, 2) != NULL
		|| ct_memcpy(str_buffer, NULL, 2) != NULL)
		return (4);
	return (EXIT_SUCCESS);
}
