#include "ct_memory.h"
#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_utils.h"

static void ctit_lfe_helper(void *data)
{
	int *val;

	val = (int *)data;
	*val += 1;
}

static int normal_listforeach(void)
{
	t_list *lst;
	int		val1;
	int		val2;
	int		val3;

	lst = NULL;
	val1 = 0;
	val2 = 0;
	val3 = 0;
	cti_list_push(&lst, &val1);
	cti_list_push(&lst, &val2);
	cti_list_push(&lst, &val3);
	cti_list_foreach(lst, ctit_lfe_helper);
	if (val1 != 1 || val2 != 1 || val3 != 1)
		return (safefree(lst->next->next), safefree(lst->next), safefree(lst),
				1);
	return (safefree(lst->next->next), safefree(lst->next), safefree(lst),
			EXIT_SUCCESS);
}

static int err_check(void)
{
	cti_list_foreach(NULL, ctit_lfe_helper);
	cti_list_foreach(NULL, NULL);
	cti_list_foreach((t_list *)1, NULL);
	return (EXIT_SUCCESS);
}

int ctit_listforeach(void)
{
	int ret;

	ret = normal_listforeach();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = err_check();
	if (ret != EXIT_SUCCESS)
		return (ret);
	return (EXIT_SUCCESS);
}
