/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:54:19 by musajid           #+#    #+#             */
/*   Updated: 2025/05/21 14:16:43 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t sizeofdst)
{
	size_t	s_dst;
	size_t	s_src;
	size_t	i;
	size_t	j;

	s_dst = ft_strlen(dst);
	s_src = ft_strlen(src);
	i = s_dst;
	j = 0;
	if (sizeofdst <= s_dst)
		return (sizeofdst + s_src);
	while (src[j] != '\0' && i < sizeofdst - 1)
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	dst[i] = '\0';
	return (s_dst + s_src);
}
