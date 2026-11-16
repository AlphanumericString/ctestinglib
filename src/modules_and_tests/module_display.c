#define CTESTS_INTERNAL
#include "ct_internal.h"

#define JSON_SILENT "ctests: json format cannot skip any field.\n"

static bool format_is(t_display_mode mode, t_display_mode format)
{
	return ((mode & CT_FMT_MSK) == format);
}

// json needs every field to be printed to be parsable.
static const char *format_error(t_display_mode mode)
{
	if (format_is(mode, CT_FMT_JSN) && (mode & CT_DM_SUMMSK) == CT_DM_SUMSILENT)
		return (JSON_SILENT);
	return (NULL);
}

// add terminating '\n' for "nice-ness"
static void display_json(const t_module *m, t_display_mode mode)
{
	cti_display_results_json(m, mode);
	cti_print_string("\n");
}

void ct_module_display(const t_module *module, t_display_mode mode)
{
	if (!module || format_is(mode, CT_FMT_SILENT))
		return;
	if (format_error(mode))
		return ((void)cti_print_string(format_error(mode)));
	switch (mode & CT_FMT_MSK)
	{
		case CT_FMT_TXT:
			return (cti_display_results_txt(module, mode, 0), (void)0);
		case CT_FMT_JSN:
			return (display_json(module, mode));
		default:
			return (cti_print_string("unsuported format\n"), (void)0);
	}
}
