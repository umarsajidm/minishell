/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 18:51:36 by musajid           #+#    #+#             */
/*   Updated: 2025/05/23 18:51:38 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*temp;
	const unsigned char	*temp2;

	if (!dst && !src && n > 0)
		return (NULL);
	temp = (unsigned char *)dst;
	temp2 = (const unsigned char *)src;
	while (n--)
		*temp++ = *temp2++;
	return (dst);
}
