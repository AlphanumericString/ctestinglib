#include "ct_memory.h"
#include <stdlib.h>

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "ct_utils.h"

int ctit_listpush(void)
{
	t_list *lst;

	lst = NULL;
	cti_list_push(&lst, (void *)1);
	if (!lst || lst->data != (void *)1 || lst->next != NULL)
		return (1);
	cti_list_push(&lst, (void *)2);
	if (!lst || lst->data != (void *)2 || !lst->next ||
		lst->next->data != (void *)1 || lst->next->next != NULL)
		return (safefree(lst), 2);
	return (safefree(lst->next), safefree(lst), EXIT_SUCCESS);
}
