/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:30:42 by bgoulard          #+#    #+#             */
/*   Updated: 2025/08/08 18:30:42 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define INTERNAL_TESTINGLIB
#include "tests_fxtr.h"

#include <stdio.h>

void	print_padding(int depth)
{
	while (depth--)
		print_string("\t");
}

void	display_module_chain(const t_module *m)
{
	if (m->parent)
		display_module_chain(m->parent);
	print_string(m->name);
	print_string(":");
}

void	print_string(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	write(STDOUT_FILENO, str, i);
}

void	print_nb(int nb)
{
	char	buffer[12];
	int		i;

	if (nb == 0)
	{
		write(STDOUT_FILENO, "0", 1);
		return ;
	}
	i = 0;
	while (nb > 0)
	{
		buffer[i++] = '0' + (nb % 10);
		nb /= 10;
	}
	while (--i >= 0)
		write(STDOUT_FILENO, &buffer[i], 1);
}

void	print_flt(float f)
{
	int		int_part;

	if (f < 0)
	{
		write(STDOUT_FILENO, "-", 1);
		f = -f;
	}
	int_part = (int)f;
	print_nb(int_part);
	write(STDOUT_FILENO, ".", 1);
	f -= int_part;
	f *= 100;
	print_nb((int)f / 10);
	print_nb((int)f % 10);
}
