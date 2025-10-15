/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 00:36:18 by musajid           #+#    #+#             */
/*   Updated: 2025/07/26 17:11:35 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr_fd(int num, int fd)
{
	char	*str;
	int		len;

	str = ft_itoa(num);
	if (!str)
		return (-1);
	len = ft_putstr_fd(str, fd);
	free(str);
	if (len == -1)
		return (-1);
	return (len);
}
