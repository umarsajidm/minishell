/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:13:38 by achowdhu          #+#    #+#             */
/*   Updated: 2025/08/09 18:44:17 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putendl_fd(char *str, int fd)
{
	int	count;

	if (!str || fd < 0)
		return (0);
	count = write(fd, str, ft_strlen(str));
	return (count + write(fd, "\n", 1));
}
