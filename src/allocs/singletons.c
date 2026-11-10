#define CTESTS_INTERNAL
#include "ct_memory.h"

int64_t *cti_failloc_get(void)
{
	static int64_t nb_before_fail = INT64_MAX;

	return (&nb_before_fail);
}

size_t *cti_alloc_get(void)
{
	static size_t nb_allocs = 0;

	return (&nb_allocs);
}
