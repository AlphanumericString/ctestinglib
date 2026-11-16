#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define CTESTS_INTERNAL
#include "ct_modules.h"
#include "ct_internal.h"

void cti_selection_display(t_module **selection, int count, t_display_mode mode)
{
	const t_display_fmt *fmt;

	if ((mode & CT_FMT_MSK) == CT_FMT_SILENT || !count)
		return;
	for (int i = 0; i < count; i++)
		ct_module_sort(selection[i]);
	fmt = cti_display_fmt_get(mode);
	if (!fmt)
		return;
	if (!fmt->md_list_displayer)
		return;
	fmt->md_list_displayer(selection, (size_t)count, mode);
}
