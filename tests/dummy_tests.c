/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dummy_tests.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 14:12:03 by bgoulard          #+#    #+#             */
/*   Updated: 2025/09/26 14:12:03 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

// smol populate
int	dummy_fail_test(void)
{
	return (EXIT_FAILURE);
}

int	dummy_success_test(void)
{
	return (EXIT_SUCCESS);
}

int	dummy_sigsegv(void)
{
	const char	*str = NULL;

	if (str[42] == 'z')
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
