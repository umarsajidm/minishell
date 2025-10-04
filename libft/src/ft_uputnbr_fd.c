/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_uputnbr_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:26:04 by achowdhu          #+#    #+#             */
/*   Updated: 2025/08/09 18:46:32 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_uputnbr_fd(unsigned int n, int fd)
{
	size_t	count;

	count = 0;
	if (n > 9)
	{
		count += ft_uputnbr_fd(n / 10, fd);
		count += ft_uputnbr_fd(n % 10, fd);
	}
	else
	{
		count += ft_putchar_fd(n + '0', fd);
	}
	return (count);
}
