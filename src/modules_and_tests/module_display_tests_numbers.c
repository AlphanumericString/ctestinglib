#define CTESTS_INTERNAL
#include "ct_modules.h"
#include "ct_utils.h"

static void	loc_mdisplay_percent_txt_and_json(int t_t, int t_p,
				t_display_mode md)
{
	const char	*str_color[] = {KO_COLOR, OK_COLOR};

	if ((md & CT_FMT_MSK) == CT_FMT_JSN)
	{
		if (!t_t)
			return (cti_print_string("\"percentage\":\"NaN%\""), (void)0);
		cti_print_string("\"percentage\":\"");
		cti_print_flt((double)t_p / t_t * 100);
		return (cti_print_string("%\""), (void)0);
	}
	if (!t_t)
		return (cti_print_string("NaN%"), (void)0);
	cti_print_string("(");
	if ((md & CT_PRTY_MSK) == CT_PRTY_COLOR)
		cti_print_string(str_color[t_p == t_t]);
	cti_print_flt((double)t_p / t_t * 100);
	if ((md & CT_PRTY_MSK) == CT_PRTY_COLOR)
		cti_print_string("\033[0m" "%)");
	return ((void)0);
}

void	cti_mdisplay_sum_percent(int t_t, int t_p, t_display_mode md)
{
	if ((md & CT_FMT_MSK) == CT_FMT_JSN
		|| (md & CT_FMT_MSK) == CT_FMT_TXT)
		return (loc_mdisplay_percent_txt_and_json(t_t, t_p, md));
	cti_print_string("Unsupported format for percentage display\n");
}

static void	loc_mdisplay_nb_txt_and_json(int t_t, int t_p, t_display_mode md)
{
	const char	*str_color[] = {KO_COLOR, OK_COLOR};

	if ((md & CT_FMT_MSK) == CT_FMT_JSN)
	{
		cti_print_string("\"nb_tests_passed\":\"");
		cti_print_nb(t_p);
		cti_print_string("\",\"nb_tests_total\":\"");
		cti_print_nb(t_t);
		return (cti_print_string("\""), (void)0);
	}
	if (!t_t)
		return (cti_print_string("empty"), (void)0);
	if ((md & CT_PRTY_MSK) == CT_PRTY_COLOR)
		cti_print_string(str_color[t_p == t_t]);
	cti_print_nb(t_p);
	if ((md & CT_PRTY_MSK) == CT_PRTY_COLOR)
		cti_print_string("\033[0m");
	return (cti_print_string(" / "), cti_print_nb(t_t), (void)0);
}

void	cti_mdisplay_sum_nb(int t_t, int t_p, t_display_mode md)
{
	if ((md & CT_FMT_MSK) == CT_FMT_JSN || (md & CT_FMT_MSK) == CT_FMT_TXT)
		return (loc_mdisplay_nb_txt_and_json(t_t, t_p, md));
	cti_print_string("Unsupported format for percentage display\n");
}
