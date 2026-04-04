/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cm_assert.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 21:51:02 by bgoulard          #+#    #+#             */
/*   Updated: 2026/03/31 21:51:02 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tc_structs.h"

#define CTESTS_INTERNAL_TESTS
#include "tc_assert.h"
#include "tc_modules.h"

#define ASRT_DSC	"Assertion module to check conditions and print on fail"

t_module	*create_assert_tests(void)
{
	t_module	*m_assert;
	const char	*desc = ASRT_DSC;

	m_assert = NULL;
	tc_module_init(&m_assert, "Module assert", desc);
	tc_module_taddf(m_assert, tcit_true, "true");
	tc_module_taddf(m_assert, tcit_false, "false");
	tc_module_taddf(m_assert, tcit_streq, "str equality checks");
	tc_module_taddf(m_assert, tcit_strne, "str differ checks");
	return (m_assert);
}

#undef ASRT_DSC
