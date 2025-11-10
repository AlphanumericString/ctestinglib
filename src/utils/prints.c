/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prints.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:36:50 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/07 15:36:50 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <limits.h>

#define CTESTS_INTERNAL
#include "tc_modules.h"
#include "tc_utils.h"

void	tci_print_string(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	write(STDOUT_FILENO, str, i);
}

void	tci_print_charn(int depth, char c)
{
	if (depth <= 0 || !c)
		return ;
	while (depth--)
		write(STDOUT_FILENO, &c, 1);
}

void	tci_print_nb(int nb)
{
	char	buffer[sizeof(int) * 8];
	int		i;

	if (nb == 0)
		return (write(STDOUT_FILENO, "0", 1), (void)0);
	if (nb == INT_MIN)
		return (write(STDOUT_FILENO, "-2147483648", 11), (void)0);
	if (nb < 0)
		nb = (write(STDOUT_FILENO, "-", 1), -nb);
	i = 0;
	while (nb != 0)
	{
		buffer[i++] = '0' + (nb % 10);
		nb /= 10;
	}
	while (--i >= 0)
		write(STDOUT_FILENO, &buffer[i], 1);
}

// prints float with 2 decimal points
void	tci_print_flt(float f)
{
	int		int_part;

	if (f < 0)
	{
		write(STDOUT_FILENO, "-", 1);
		f = -f;
	}
	int_part = (int)f;
	tci_print_nb(int_part);
	write(STDOUT_FILENO, ".", 1);
	f -= int_part;
	f *= 100;
	tci_print_nb((int)f / 10);
	if (f - (int)f >= 0.5f)
		f += 1.0f;
	tci_print_nb((int)f % 10);
}
