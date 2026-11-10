#include <stdlib.h>

#define CTESTS_INTERNAL_TESTS
#include "ct_utils.h"

int ctit_memchr(void)
{
	char   str[32 + 1];
	size_t i;

	i = 0;
	while (i < 32)
	{
		str[i] = '0' + (i % 10);
		if (str[i++] == '9')
			str[i - 1] = '8';
	}
	str[32 / 2] = '9';
	str[32] = 0;
	if (ct_memchr(str, '8', 32) != &str[8] ||
		ct_memchr(str, '7', 32) != &str[7] ||
		ct_memchr(str, '9', 32) != &str[32 / 2])
		return (1);
	if (ct_memchr(str + 3, '8', 32 - 3) != &str[8] ||
		ct_memchr(str + 3, '9', 32 - 3) != &str[32 / 2])
		return (2);
	if (ct_memchr(str, 'z', sizeof(size_t) + 2) || ct_memchr(str, 'z', 32) ||
		ct_memchr(str, '1', 0) || ct_memchr(str, 4242, 1) ||
		ct_memchr(NULL, 42, 1) || ct_memchr(str, -42, 1))
		return (3);
	return (EXIT_SUCCESS);
}
