/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_display.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:26:09 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/14 00:31:43 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CTESTS_INTERNAL
#include "tc_modules.h"
#include "tc_utils.h"

#define JSON_SILENT "ctests: json format cannot skip any field.\n"

// error 1: json + any silent field mode
static int	format_error(t_display_mode mode)
{
	if ((mode & TC_FMT_MSK) == TC_FMT_JSN
		&& (mode & TC_DM_SUMMSK) == TC_DM_SUMSILENT)
		return (1);
	return (0);
}

void	tc_module_display(const t_module *module, t_display_mode mode)
{
	const char	*err_strings[] = {"", JSON_SILENT};

	if (!module || (mode & TC_FMT_MSK) == TC_FMT_SILENT)
		return ;
	if (format_error(mode))
		return (tci_print_string(err_strings[format_error(mode)]), (void)0);
	if ((mode & TC_FMT_MSK) == TC_FMT_TXT)
		return (tci_display_results_txt(module, mode, 0), (void) 0);
	if ((mode & TC_FMT_MSK) == TC_FMT_JSN)
		return (tci_display_results_json(module, mode), (void)0);
	return (tci_print_string("unsuported format\n"), (void)0);
}
