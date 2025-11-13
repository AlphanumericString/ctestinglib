/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_mem.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 00:09:45 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/14 00:32:30 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tc_utils.h"

void	tc_memset(void *ptr, int value, size_t size)
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

int	tc_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*ptr1;
	const unsigned char	*ptr2;
	size_t				i;

	if (s1 == s2 || !n)
		return (0);
	if (!s1 || !s2)
		return ((s2 == NULL) - (s1 == NULL));
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

void	*tc_memcpy(void	*d, const void	*s, size_t n)
{
	unsigned char		*dest;
	const unsigned char	*src;
	size_t				i;

	if (!d || !s)
		return (NULL);
	dest = (unsigned char *)d;
	src = (const unsigned char *)s;
	i = 0;
	while (i < n)
	{
		dest[i] = src[i];
		i++;
	}
	return (d);
}

void	*tc_memmove(void *d, const void *s, size_t n)
{
	char		*dest;
	const char	*src;
	size_t		i;

	if (!d || !s)
		return (d);
	dest = (char *)d;
	src = (const char *)s;
	if (dest < src)
		return (tc_memcpy(d, s, n));
	i = n;
	while (i > 0)
	{
		i--;
		dest[i] = src[i];
	}
	return (d);
}

void	*tc_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*ptr;
	size_t				i;

	ptr = (const unsigned char *)s;
	i = 0;
	if (!s || !n)
		return (NULL);
	while (i < n)
	{
		if (ptr[i] == (unsigned char)c)
			return ((void *)(ptr + i));
		i++;
	}
	return (NULL);
}
