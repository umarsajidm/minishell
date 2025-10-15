/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 00:30:50 by musajid           #+#    #+#             */
/*   Updated: 2025/07/26 17:10:30 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putstr_fd(char *s, int fd)
{
	int	i;
	int	written;

	i = 0;
	if (!s)
		return (write(1, "(null)", 6));
	while (s[i])
	{
		written = write(fd, &s[i], 1);
		if (written == -1)
			return (-1);
		i++;
	}
	return (i);
}
