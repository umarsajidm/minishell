/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:11:16 by achowdhu          #+#    #+#             */
/*   Updated: 2025/12/18 12:36:34 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putstr_fd(char *str, int fd)
{
	int	write_result;

	if (!str || fd < 0)
		return (0);
	write_result = write(fd, str, ft_strlen(str));
	if (write_result < 0)
		return (-1);
	return (write_result);
}
