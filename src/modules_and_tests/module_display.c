#define CTESTS_INTERNAL
#include "ct_internal.h"

#define JSON_SILENT "ctests: json format cannot skip any field.\n"
#define FMT_UNKNOWN "ctests: unknown format for display used"

const t_display_fmt *cti_display_fmt_get(t_display_mode md)
{
	switch (md & CT_FMT_MSK)
	{
		case CT_FMT_TXT:
			return (&g_cti_fmt_txt);
		case CT_FMT_JSN:
			return (&g_cti_fmt_json);
		default:
			return (NULL);
	}
}

static void run_hook(t_display_hook hook, const t_module *m, t_display_mode md,
					 int depth)
{
	if (hook)
		hook(m, md, depth);
}

void cti_display_module(const t_module *m, t_display_mode md, int depth)
{
	const t_display_fmt *fmt;

	fmt = cti_display_fmt_get(md);
	if (!fmt)
		return;
	run_hook(fmt->open, m, md, depth);
	if (fmt->submodules_first)
	{
		run_hook(fmt->modules, m, md, depth);
		run_hook(fmt->tests, m, md, depth);
	}
	else
	{
		run_hook(fmt->tests, m, md, depth);
		run_hook(fmt->modules, m, md, depth);
	}
	run_hook(fmt->summary, m, md, depth);
	run_hook(fmt->close, m, md, depth);
}

void cti_display_run(const t_module *m, t_display_mode md)
{
	const t_display_fmt *fmt;

	fmt = cti_display_fmt_get(md);
	if (!fmt)
		return ((void)cti_print_string("unsuported format\n"));
	run_hook(fmt->first_open, m, md, 0);
	cti_display_module(m, md, 0);
	run_hook(fmt->last_close, m, md, 0);
}

static bool format_is(t_display_mode mode, t_display_mode format)
{
	return ((mode & CT_FMT_MSK) == format);
}

// json needs every field to be printed to be parsable.
static const char *format_error(t_display_mode mode)
{
	if (format_is(mode, CT_FMT_JSN) && (mode & CT_DM_SUMMSK) == CT_DM_SUMSILENT)
		return (JSON_SILENT);
	if (cti_display_fmt_get(mode) == NULL)
		return (NULL);
	return (NULL);
}

void ct_module_display(const t_module *module, t_display_mode mode)
{
	const char *error;

	if (!module || format_is(mode, CT_FMT_SILENT))
		return;
	error = format_error(mode);
	if (error)
		return ((void)cti_print_string(error));
	cti_display_run(module, mode);
}

// legacy hooks for internal test-suite.
void cti_display_results_txt(const t_module *m, t_display_mode md, int depth)
{
	cti_display_module(m, md, depth);
}

void cti_display_results_json(const t_module *m, t_display_mode md)
{
	cti_display_module(m, md, 0);
}
