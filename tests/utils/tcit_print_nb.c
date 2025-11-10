/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcit_print_nb.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 01:40:30 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/08 01:40:30 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

#include "tc_capture.h"

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "tc_utils.h"

int	tcit_print_nb(void)
{
	t_capture	*cap;

	cap = tc_capture_create(STDOUT_FILENO);
	tci_print_nb(0);
	tci_print_nb(42);
	tci_print_nb(99999);
	if (tc_capture_compare(cap, "04299999") != 0)
		return (tc_capture_destroy(cap), 1);
	tci_print_nb(INT_MAX);
	tci_print_nb(INT_MIN);
	tci_print_nb(INT_MIN + 2);
	if (tc_capture_compare(cap, "2147483647-2147483648-2147483646") != 0)
		return (tc_capture_destroy(cap), 2);
	return (tc_capture_destroy(cap), EXIT_SUCCESS);
}
