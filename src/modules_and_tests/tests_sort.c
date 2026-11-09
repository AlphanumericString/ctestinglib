#include <stddef.h>

#define CTESTS_INTERNAL
#include "ct_modules.h"
#include "ct_utils.h"

static t_list	*grab_min(t_list *lst)
{
	t_list	*min;

	min = NULL;
	while (lst)
	{
		if (!min || cti_cmp_alpha(((t_test *)min->data)->name,
				((t_test *)lst->data)->name) > 0)
			min = lst;
		lst = lst->next;
	}
	return (min);
}

void	sort_testslist(t_list *t_ls)
{
	t_list	*min;
	t_test	*data;

	min = NULL;
	while (t_ls)
	{
		min = grab_min(t_ls);
		if (min && min != t_ls)
		{
			data = t_ls->data;
			t_ls->data = min->data;
			min->data = data;
		}
		t_ls = t_ls->next;
		min = NULL;
	}
}
