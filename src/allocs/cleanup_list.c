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

// Check whether ptr is currently registered for automatic cleanup.
// Returns true when an entry points to ptr, false otherwise.
bool is_in_cleanup(void *ptr)
{
	t_list *leaf;

	leaf = *cleanup_list();
	while (leaf)
	{
		if (leaf->data == ptr)
			return (true);
		leaf = leaf->next;
	}
	return (false);
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

// Release every registered pointer with f (free() by default).
// Each node is unlinked before f() is called so that a custom cleanup function
// may free its argument (with free() or safefree()) without hitting a stale
// node or a second free when the cleanup walk resumes.
void do_cleanup(void (*f)(void *))
{
	t_list **list;
	t_list	*leaf;
	void	*data;

	if (!f)
		f = __libc_free;
	list = cleanup_list();
	while (*list)
	{
		leaf = *list;
		data = leaf->data;
		*list = leaf->next;
		__libc_free(leaf);
		f(data);
	}
}
