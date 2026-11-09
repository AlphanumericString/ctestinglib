#define CTESTS_INTERNAL
#include "ct_structs.h"
#include "ct_utils.h"
#include "ct_memory.h"

// lst //
void	cti_list_foreach(const t_list *lst, void (*f)(void *data))
{
	if (!lst || !f)
		return ;
	while (lst)
	{
		f(lst->data);
		lst = lst->next;
	}
}

void	cti_list_push(t_list **lst, void *data)
{
	t_list	*leaf;

	leaf = safealloc(sizeof(*leaf));
	if (!leaf)
		return ;
	ct_memset(leaf, 0, sizeof(*leaf));
	leaf->data = data;
	leaf->next = *lst;
	*lst = leaf;
}

// pop
void	*cti_list_pop(t_list **lst)
{
	t_list	*temp;
	void	*data;

	if (!lst || !*lst)
		return (NULL);
	temp = *lst;
	data = temp->data;
	*lst = (*lst)->next;
	safefree(temp);
	return (data);
}
