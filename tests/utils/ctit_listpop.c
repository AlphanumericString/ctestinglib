#include "ct_memory.h"
#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_utils.h"

static int	normal_list_pop(void)
{
	t_list	*lst;
	void	*data;

	lst = NULL;
	if (cti_list_pop(&lst) != NULL)
		return (1);
	cti_list_push(&lst, (void *)1);
	cti_list_push(&lst, (void *)2);
	data = cti_list_pop(&lst);
	if (data != (void *)2)
		return (safefree(lst), 2);
	data = cti_list_pop(&lst);
	if (data != (void *)1)
		return (safefree(lst), 3);
	if (cti_list_pop(&lst) != NULL)
		return (4);
	return (EXIT_SUCCESS);
}

static int	err_check(void)
{
	t_list	*lst;
	void	*data;

	lst = NULL;
	data = cti_list_pop(NULL);
	if (data != NULL)
		return (1);
	data = cti_list_pop(&lst);
	if (data != NULL)
		return (2);
	return (EXIT_SUCCESS);
}

int	ctit_listpop(void)
{
	int	ret;

	ret = normal_list_pop();
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = err_check();
	return (ret);
}
