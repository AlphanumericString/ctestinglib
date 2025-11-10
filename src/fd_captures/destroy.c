/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 12:31:31 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/02 12:31:31 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tc_capture.h"
#define CTESTS_INTERNAL
#include "tc_memory.h"

void	tc_capture_destroy(t_capture *c)
{
	if (!c)
		return ;
	tc_capture_release(c);
	safefree(c);
}
