#define CTESTS_INTERNAL
#include "ct_memory.h"
#include "ct_utils.h"

extern void	__libc_free(void *ptr);

static t_list	**cleanup_list(void)
{
	static t_list	*list = NULL;

	return (&list);
}

void	push_to_cleanup(void *ptr)
{
	t_list	**ptr_list;

	ptr_list = cleanup_list();
	return (cti_list_push(ptr_list, ptr));
}

void	do_cleanup(void (*f)(void *))
{
	t_list	*leaf;
	t_list	*nxt;

	if (!f)
		f = __libc_free;
	leaf = *cleanup_list();
	while (leaf)
	{
		f(leaf->data);
		nxt = leaf->next;
		__libc_free(leaf);
		leaf = nxt;
	}
	*cleanup_list() = NULL;
}
