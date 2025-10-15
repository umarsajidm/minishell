/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 18:50:16 by musajid           #+#    #+#             */
/*   Updated: 2025/05/23 18:50:18 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t			i;
	unsigned char	*temp;

	if (nmemb != 0 && size > SIZE_MAX / nmemb)
		return (NULL);
	temp = (unsigned char *)malloc(nmemb * size);
	if (!temp)
		return (NULL);
	i = 0;
	while (i < nmemb * size)
		temp[i++] = 0;
	return ((void *)temp);
}
