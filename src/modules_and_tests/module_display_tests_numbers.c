#define CTESTS_INTERNAL
#include "ct_internal.h"

static void loc_percent_txt(int t_t, int t_p, t_display_mode md)
{
	const char *str_color[] = {KO_COLOR, OK_COLOR};

	if (!t_t)
		return ((void)cti_print_string("NaN%"));
	cti_print_string("(");
	if ((md & CT_PRTY_MSK) == CT_PRTY_COLOR)
		cti_print_string(str_color[t_p == t_t]);
	cti_print_flt((double)t_p / t_t * 100);
	if ((md & CT_PRTY_MSK) == CT_PRTY_COLOR)
		cti_print_string("\033[0m"
						 "%)");
}

static void loc_percent_json(int t_t, int t_p)
{
	if (!t_t)
		return ((void)cti_print_string("\"percentage\":\"NaN%\""));
	cti_print_string("\"percentage\":\"");
	cti_print_flt((double)t_p / t_t * 100);
	cti_print_string("%\"");
}

void cti_mdisplay_sum_percent(int t_t, int t_p, t_display_mode md)
{
	switch (md & CT_FMT_MSK)
	{
		case CT_FMT_TXT:
			return (loc_percent_txt(t_t, t_p, md));
		case CT_FMT_JSN:
			return (loc_percent_json(t_t, t_p));
		default:
			return ((void)cti_print_string(
				"Unsupported format for percentage display\n"));
	}
}

static void loc_nb_txt(int t_t, int t_p, t_display_mode md)
{
	const char *str_color[] = {KO_COLOR, OK_COLOR};

	if (!t_t)
		return ((void)cti_print_string("empty"));
	if ((md & CT_PRTY_MSK) == CT_PRTY_COLOR)
		cti_print_string(str_color[t_p == t_t]);
	cti_print_nb(t_p);
	if ((md & CT_PRTY_MSK) == CT_PRTY_COLOR)
		cti_print_string("\033[0m");
	cti_print_string(" / ");
	cti_print_nb(t_t);
}

static void loc_nb_json(int t_t, int t_p)
{
	cti_print_string("\"nb_tests_passed\":\"");
	cti_print_nb(t_p);
	cti_print_string("\",\"nb_tests_total\":\"");
	cti_print_nb(t_t);
	cti_print_string("\"");
}

void cti_mdisplay_sum_nb(int t_t, int t_p, t_display_mode md)
{
	switch (md & CT_FMT_MSK)
	{
		case CT_FMT_TXT:
			return (loc_nb_txt(t_t, t_p, md));
		case CT_FMT_JSN:
			return (loc_nb_json(t_t, t_p));
		default:
			return ((void)cti_print_string(
				"Unsupported format for percentage display\n"));
	}
}
