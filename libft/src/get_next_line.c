/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 17:36:38 by achowdhu          #+#    #+#             */
/*   Updated: 2025/08/17 15:07:39 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;
	size_t	len1;
	size_t	len2;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	result = (char *)malloc(len1 + len2 + 1);
	if (!result)
	{
		free(s1);
		return (NULL);
	}
	ft_memcpy(result, s1, len1);
	ft_memcpy(result + len1, s2, len2 + 1);
	free(s1);
	return (result);
}

static int	read_to_buffer(int fd, char *buffer)
{
	int	bytes_read;

	bytes_read = read(fd, buffer, BUFFER_SIZE);
	if (bytes_read < 0)
		return (-1);
	buffer[bytes_read] = '\0';
	return (bytes_read);
}

static char	*extract_line(char *buffer, char *line)
{
	char	*nl_pos;
	char	*temp;

	nl_pos = ft_strchr(buffer, '\n');
	if (nl_pos)
	{
		temp = ft_substr(buffer, 0, nl_pos - buffer + 1);
		if (!temp)
			return (free(line), NULL);
		ft_memmove(buffer, nl_pos + 1, ft_strlen(nl_pos + 1) + 1);
	}
	else
	{
		temp = ft_strdup(buffer);
		if (!temp)
			return (free(line), NULL);
		*buffer = '\0';
	}
	if (!line)
		return (temp);
	line = ft_strjoin_free(line, temp);
	free(temp);
	return (line);
}

static char	*read_until_newline(int fd, char *buffer, char *line)
{
	ssize_t	bytes_read;

	while (1)
	{
		bytes_read = read_to_buffer(fd, buffer);
		if (bytes_read <= 0)
		{
			if (bytes_read < 0)
				return (free(line), NULL);
			return (line);
		}
		line = extract_line(buffer, line);
		if (!line || ft_strchr(line, '\n'))
			break ;
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
		return (NULL);
	line = NULL;
	if (*buffer)
	{
		line = extract_line(buffer, NULL);
		if (!line)
			return (NULL);
		if (ft_strchr(line, '\n'))
			return (line);
	}
	return (read_until_newline(fd, buffer, line));
}
