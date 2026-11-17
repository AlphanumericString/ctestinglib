#define CTESTS_INTERNAL
#include "ct_internal.h"

extern void __libc_free(void *ptr);

static t_list **cleanup_list(void)
{
	static t_list *list = NULL;

	return (&list);
}

void push_to_cleanup(void *ptr)
{
	t_list **ptr_list;

	ptr_list = cleanup_list();
	return (cti_list_push(ptr_list, ptr));
}

// Remove the first entry pointing to ptr.
// Returns true when an entry was found and removed, false otherwise.
bool remove_from_cleanup(void *ptr)
{
	t_list **list;
	t_list	*leaf;
	t_list	*prev;

	list = cleanup_list();
	prev = NULL;
	leaf = *list;
	while (leaf)
	{
		if (leaf->data == ptr)
		{
			if (prev)
				prev->next = leaf->next;
			else
				*list = leaf->next;
			__libc_free(leaf);
			return (true);
		}
		prev = leaf;
		leaf = leaf->next;
	}
	return (false);
}

void do_cleanup(void (*f)(void *))
{
	t_list *leaf;
	t_list *nxt;

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
