/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 18:51:54 by musajid           #+#    #+#             */
/*   Updated: 2025/05/23 18:51:56 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	int_len(int n);

char	*ft_itoa(int n)
{
	size_t	len;
	char	*str;
	long	num;

	num = n;
	len = int_len(n);
	str = (char *)ft_calloc((len + 1), sizeof(char));
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (num < 0)
	{
		str[0] = '-';
		num = -num;
	}
	if (num == 0)
		str[0] = '0';
	while (num != 0)
	{
		str[--len] = num % 10 + 48;
		num = num / 10;
	}
	return (str);
}

static size_t	int_len(int n)
{
	long	num;
	size_t	i;

	num = n;
	i = 0;
	if (num == 0)
		return (1);
	if (num < 0)
	{
		i++;
		num = -num;
	}
	while (num > 0)
	{
		num /= 10;
		i++;
	}
	return (i);
}
