/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 00:26:27 by musajid           #+#    #+#             */
/*   Updated: 2025/05/21 14:16:07 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char			**ft_split(const char *str, char del);

static size_t	word_count(const char *str, char del)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (*str)
	{
		if (*str != del && j == 0)
		{
			j = 1;
			i++;
		}
		else if (*str == del)
			j = 0;
		str++;
	}
	return (i);
}

static char	*word_extraction(const char *str, char del)
{
	size_t	i;

	i = 0;
	while (str[i] && str[i] != del)
		i++;
	return (ft_substr(str, 0, i));
}

static void	free_split(char	**arr, size_t i)
{
	while (i > 0)
	{
		free(arr[i - 1]);
		--i;
	}
	free(arr);
}

char	**ft_split(const char *str, char del)
{
	char	**arr;
	size_t	words;
	size_t	i;

	i = 0;
	words = word_count(str, del);
	if (!str)
		return (NULL);
	arr = (char **)ft_calloc((words + 1), sizeof(char *));
	if (!arr)
		return (NULL);
	while (i < words)
	{
		while (*str == del)
			str++;
		arr[i] = word_extraction(str, del);
		if (!arr[i])
			return (free_split(arr, i), NULL);
		while (*str && *str != del)
			str++;
		i++;
	}
	return (arr);
}
