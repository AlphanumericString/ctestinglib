/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmp_alpha.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 23:17:27 by bgoulard          #+#    #+#             */
/*   Updated: 2025/09/16 23:17:27 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define INTERNAL_TESTINGLIB
#include "tests_fxtr.h"

int	_cmp_alpha(const char *s1, const char *s2)
{
	size_t	len1;
	size_t	len2;

	if (s1 == s2)
		return (0);
	if (!s1 || !s2)
		return ((s2 == NULL) - (s1 == NULL));
	len1 = 0;
	len2 = 0;
	while (s1[len1] && s1[len1] == s2[len2])
	{
		len1++;
		len2++;
	}
	return ((unsigned char)s1[len1] - (unsigned char)s2[len2]);
}
