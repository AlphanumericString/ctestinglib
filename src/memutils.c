/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memutils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 00:09:45 by bgoulard          #+#    #+#             */
/*   Updated: 2025/08/15 00:09:45 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memutils.h"

void	self_memset(void *ptr, int value, size_t size)
{
	unsigned char	*byte_ptr;
	size_t			i;

	byte_ptr = (unsigned char *)ptr;
	i = 0;
	while (i < size)
	{
		byte_ptr[i] = (unsigned char)value;
		i++;
	}
}

int	self_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*ptr1;
	const unsigned char	*ptr2;
	size_t				i;

	ptr1 = (const unsigned char *)s1;
	ptr2 = (const unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (ptr1[i] != ptr2[i])
			return (ptr1[i] - ptr2[i]);
		i++;
	}
	return (0);
}
