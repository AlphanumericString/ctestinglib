#define CTESTS_INTERNAL
#include "ct_modules.h"
#include "ct_utils.h"

#define JSON_SILENT "ctests: json format cannot skip any field.\n"

// error 1: json + any silent field mode
static int format_error(t_display_mode mode)
{
	if ((mode & CT_FMT_MSK) == CT_FMT_JSN &&
		(mode & CT_DM_SUMMSK) == CT_DM_SUMSILENT)
		return (1);
	return (0);
}

void ct_module_display(const t_module *module, t_display_mode mode)
{
	const char *err_strings[] = {"", JSON_SILENT};

	if (!module || (mode & CT_FMT_MSK) == CT_FMT_SILENT)
		return;
	if (format_error(mode))
		return (cti_print_string(err_strings[format_error(mode)]), (void)0);
	if ((mode & CT_FMT_MSK) == CT_FMT_TXT)
		return (cti_display_results_txt(module, mode, 0), (void)0);
	if ((mode & CT_FMT_MSK) == CT_FMT_JSN)
		return (cti_display_results_json(module, mode), cti_print_string("\n"),
				(void)0);
	return (cti_print_string("unsuported format\n"), (void)0);
}

// reports a module selection as one json array so the output stays valid
// json when several modules are selected
void cti_display_results_json_list(t_module **modules, size_t count,
								   t_display_mode md)
{
	size_t i;

	if ((md & CT_FMT_MSK) == CT_FMT_SILENT)
		return;
	if (format_error(md))
		return (cti_print_string(JSON_SILENT), (void)0);
	cti_print_string("[");
	i = 0;
	while (i < count)
	{
		cti_display_results_json(modules[i], md);
		if (++i < count)
			cti_print_string(",");
	}
	cti_print_string("]\n");
}
