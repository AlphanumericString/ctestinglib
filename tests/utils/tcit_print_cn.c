/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcit_printcn.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:38:56 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/07 15:38:56 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "tc_capture.h"

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "tc_utils.h"

int	tcit_print_cn(void)
{
	t_capture	*cap;
	int			ret;

	cap = tc_capture_create(STDOUT_FILENO);
	tci_print_charn(5, '*');
	tci_print_charn(0, '*');
	tci_print_charn(-2, '*');
	tci_print_charn(99999, '\0');
	ret = EXIT_SUCCESS;
	if (tc_capture_compare(cap, "*****") != 0)
		ret = EXIT_FAILURE;
	tc_capture_destroy(cap);
	return (ret);
}
