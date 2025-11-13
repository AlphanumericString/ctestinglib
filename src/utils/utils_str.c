/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 00:26:35 by bgoulard          #+#    #+#             */
/*   Updated: 2025/11/14 00:32:34 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tc_utils.h"

int	tc_strncmp(const char *s, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && s[i] == s2[i] && s[i])
		i++;
	if (i == n)
		return (0);
	return ((unsigned char)s[i] - (unsigned char)s2[i]);
}

size_t	tc_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}
