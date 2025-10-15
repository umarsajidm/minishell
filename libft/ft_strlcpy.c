/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 18:52:16 by musajid           #+#    #+#             */
/*   Updated: 2025/05/23 18:52:17 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t sizeofdst)
{
	size_t	s_src;

	s_src = ft_strlen(src);
	if (sizeofdst == 0)
		return (s_src);
	else if (s_src < sizeofdst)
	{
		ft_memcpy(dst, src, s_src);
		dst[s_src] = '\0';
	}
	else if (sizeofdst <= s_src)
	{
		ft_memcpy(dst, src, sizeofdst - 1);
		dst[sizeofdst - 1] = '\0';
	}
	return (s_src);
}
