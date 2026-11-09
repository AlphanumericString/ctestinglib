/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cm_capture.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 15:20:00 by bgoulard          #+#    #+#             */
/*   Updated: 2026/01/22 15:20:00 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CTESTS_INTERNAL_TESTS
#include "tc_modules.h"

#define CAPTURE_DSC	"Check on fd capture functionalities"

t_module	*create_capture_tests(void)
{
	t_module	*m_capture;
	const char	*desc = CAPTURE_DSC;

	m_capture = NULL;
	tc_module_init(&m_capture, "capture", desc);
	if (!m_capture)
		return (NULL);
	tc_module_taddf(m_capture, tcit_capture, "fd capture");
	return (m_capture);
}

#undef CAPTURE_DSC
