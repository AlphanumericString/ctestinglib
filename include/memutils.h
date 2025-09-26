/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memutils.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoulard <bgoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 00:10:03 by bgoulard          #+#    #+#             */
/*   Updated: 2025/08/15 00:10:03 by bgoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMUTILS_H
# define MEMUTILS_H

# include <stddef.h>

void	self_memset(void *ptr, int value, size_t size);
int		self_memcmp(const void *s1, const void *s2, size_t n);

#endif /* MEMUTILS_H */
