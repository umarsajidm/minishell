/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:03:15 by achowdhu          #+#    #+#             */
/*   Updated: 2025/04/24 20:00:41 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)

{
	size_t	dl;
	size_t	sl;
	size_t	i;

	dl = 0;
	sl = 0;
	i = 0;
	while (dl < size && dest[dl] != '\0')
	{
		dl++;
	}
	while (src[sl] != '\0')
	{
		sl++;
	}
	if (size <= dl)
		return (size + sl);
	while (src[i] != '\0' && (dl + i + 1) < size)
	{
		dest[dl + i] = src[i];
		i++;
	}
	if ((dl + i) < size)
		dest[dl + i] = '\0';
	return (dl + sl);
}
