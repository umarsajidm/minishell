/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:11:16 by achowdhu          #+#    #+#             */
/*   Updated: 2025/08/09 18:44:46 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putstr_fd(char *str, int fd)
{
	int	count;
	int	write_result;

	if (!str || fd < 0)
		return (0);
	count = 0;
	write_result = write(fd, str, ft_strlen(str));
	if (write_result < 0)
		return (-1);
	return (write_result);
}
