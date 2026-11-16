#define CTESTS_INTERNAL
#include "ct_internal.h"

static void module_destroy_inner(t_module *m)
{
	const t_list *sm_lst = m->submodules_list;
	const t_list *ts_lst = m->tests_list;
	t_list		 *tmp;

	while (sm_lst)
	{
		ct_module_destroy(sm_lst->data);
		tmp = (t_list *)sm_lst;
		sm_lst = sm_lst->next;
		safefree(tmp);
	}
	while (ts_lst)
	{
		tmp = (t_list *)ts_lst;
		ts_lst = ts_lst->next;
		safefree(tmp->data);
		safefree(tmp);
	}
	safefree(m);
	return;
}

/// @brief Destroy a module and everything it owns.
/// @details Recursively destroys the submodules, then frees the module's tests
/// and the module itself. The strings given to ct_module_init() and
/// ct_module_taddf() are not freed.
/// @param m Module to destroy, NULL is accepted.
void ct_module_destroy(t_module *m)
{
	if (!m)
		return;
	module_destroy_inner(m);
}

/// @brief Create a new, empty module.
/// @details The module is allocated and initialised with @p name and
/// @p desc. Both strings are stored as is and must outlive the module.
/// @param module Output pointer set to the new module on success.
/// @param name Name of the module, must not be NULL.
/// @param desc Description of the module, may be NULL.
/// @note On failure nothing is written to @p module.
void ct_module_init(t_module **module, const char *name, const char *desc)
{
	t_module *ret;

	if (!module || !name)
		return;
	ret = safealloc(sizeof(t_module));
	if (!ret)
		return;
	*ret = (t_module){0};
	ret->name = name;
	ret->description = desc;
	*module = ret;
}
