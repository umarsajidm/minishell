/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:29:40 by achowdhu          #+#    #+#             */
/*   Updated: 2025/04/25 20:01:17 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_count(const char *s, char c)
{
	size_t	substr;

	substr = 0;
	while (*s)
	{
		if (*s != c)
		{
			++substr;
			while (*s && *s != c)
				++s;
		}
		else
			++s;
	}
	return (substr);
}

char	**ft_split(const char *s, char c)
{
	char	**substr;
	size_t	i;
	size_t	len;

	if (!s)
		return (0);
	i = 0;
	substr = malloc(sizeof(char *) * (ft_count(s, c) + 1));
	if (!substr)
		return (0);
	while (*s)
	{
		if (*s != c)
		{
			len = 0;
			while (*s && *s != c && ++len)
				++s;
			substr[i++] = ft_substr(s - len, 0, len);
		}
		else
			++s;
	}
	substr[i] = 0;
	return (substr);
}
