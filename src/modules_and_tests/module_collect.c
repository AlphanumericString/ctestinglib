#include "ct_utils.h"
#define CTESTS_INTERNAL
#include "ct_modules.h"

int	cti_collect_sbm_t_success(const t_module	*m)
{
	t_list	*md_l;
	int		nb;

	nb = 0;
	md_l = m->submodules_list;
	while (md_l)
	{
		nb += cti_collect_sbm_t_success(md_l->data);
		md_l = md_l->next;
	}
	return (nb + m->tests_passed);
}

int	cti_collect_sbm_t_total(const t_module *m)
{
	t_list	*md_l;
	int		nb;

	nb = 0;
	md_l = m->submodules_list;
	while (md_l)
	{
		nb += cti_collect_sbm_t_total(md_l->data);
		md_l = md_l->next;
	}
	return (nb + m->tests_count);
}

size_t	cti_mget_longest_name(const t_module *m)
{
	t_list	*tl;
	size_t	i;
	size_t	tmp;

	i = 0;
	tl = m->tests_list;
	while (tl && tl->data)
	{
		tmp = ct_strlen(((const t_test *)tl->data)->name);
		if (tmp > i)
			i = tmp;
		tl = tl->next;
	}
	return (i);
}
