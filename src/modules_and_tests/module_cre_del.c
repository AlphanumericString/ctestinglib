#define CTESTS_INTERNAL
#include "ct_modules.h"
#include "ct_memory.h"

static void	module_destroy_inner(t_module *m)
{
	const t_list	*sm_lst = m->submodules_list;
	const t_list	*ts_lst = m->tests_list;
	t_list			*tmp;

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
	return ;
}

void	ct_module_destroy(t_module *m)
{
	if (!m)
		return ;
	module_destroy_inner(m);
}

void	ct_module_init(t_module **module, const char *name, const char *desc)
{
	t_module	*ret;

	if (!module || !name)
		return ;
	ret = safealloc(sizeof(t_module));
	if (!ret)
		return ;
	*ret = (t_module){0};
	ret->name = name;
	ret->description = desc;
	*module = ret;
}
