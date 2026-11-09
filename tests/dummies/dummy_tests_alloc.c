#include <stdlib.h>
#include <sys/cdefs.h>

#define CTESTS_INTERNAL_TESTS
#include "ct_modules.h"

int	dummy_alloc(void)
{
	void	*adress;

	adress = malloc(42);
	if (adress)
		return (free(adress), EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int	dummy_leak(void)
{
	void __attribute_maybe_unused__	*adress;

	adress = malloc(42);
	return (EXIT_SUCCESS);
}
