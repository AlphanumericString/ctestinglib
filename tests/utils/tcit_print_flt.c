/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcit_print_flt.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 01:59:05 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/08 01:59:05 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "tc_capture.h"
#include "tc_memory.h"
#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "tc_utils.h"

int	tcit_print_flt(void)
{
	t_capture	*cap;
	char		*s;

	cap = tc_capture_create(STDOUT_FILENO);
	tci_print_flt(0.0f);
	tci_print_flt(42.42f);
	tci_print_flt(-42.42f);
	s = tc_capture_get(cap);
	tc_capture_destroy(cap);
	if (!s)
		return (1);
	if (tc_memcmp(s, "0.0042.42-42.42", 15) != 0)
		return (safefree(s), 2);
	safefree(s);
	return (EXIT_SUCCESS);
}
