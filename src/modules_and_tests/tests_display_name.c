#define CTESTS_INTERNAL
#include "ct_internal.h"

void cti_mdisplay_chain(const t_module *m)
{
	if (m->parent)
		cti_mdisplay_chain(m->parent);
	cti_print_string(m->name);
	cti_print_string(":");
}

void cti_mdisplay_test_name(const t_test *t, const t_module *m,
							t_display_mode md)
{
	if ((md & CT_DT_NAMEMSK) == CT_DT_FULLNAME)
		cti_mdisplay_chain(m);
	cti_print_string(t->name);
}
