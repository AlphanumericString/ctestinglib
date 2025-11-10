/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcit_print_str.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 21:33:01 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/08 21:33:01 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

#include "tc_capture.h"
#include "tc_memory.h"

#define CTESTS_INTERNAL
#define CTESTS_INTERNAL_TESTS
#include "tc_utils.h"

#define TEST_STRING "this\nis\va\rtEst\"tsT\'on_@prINTing!STRINgs"

int	tcit_print_string(void)
{
	t_capture	*cap;
	char		*s;

	cap = tc_capture_create(STDOUT_FILENO);
	tci_print_string(TEST_STRING);
	s = tc_capture_get(cap);
	tc_capture_destroy(cap);
	if (!s)
		return (1);
	if (tc_memcmp(s, TEST_STRING, tc_strlen(TEST_STRING)) != 0)
		return (safefree(s), 2);
	return (safefree(s), EXIT_SUCCESS);
}

#undef TEST_STRING
