/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 11:58:48 by musajid           #+#    #+#             */
/*   Updated: 2025/08/06 13:16:24 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_print_hex(unsigned long n, int is_uppercase)
{
	char	*hex;
	int		count;
	int		printed;

	count = 0;
	if (is_uppercase)
		hex = "0123456789ABCDEF";
	else
		hex = "0123456789abcdef";
	if (n >= 16)
		count += ft_print_hex(n / 16, is_uppercase);
	printed = ft_putchar_fd(hex[n % 16], 1);
	if (printed == -1)
		return (-1);
	count += printed;
	return (count);
}

int	ft_pointer(void *pointer)
{
	uintptr_t	address;
	int			count;
	int			printed;

	count = 0;
	address = (uintptr_t)pointer;
	if (pointer == 0)
		count += ft_putstr_fd("(nil)", 1);
	else
	{
		printed = ft_putstr_fd("0x", 1);
		if (printed == -1)
			return (-1);
		count += printed;
		printed = ft_print_hex(address, 0);
		if (printed == -1)
			return (-1);
		count += printed;
	}
	return (count);
}

static size_t	int_len(unsigned int num)
{
	size_t	i;

	i = 0;
	if (num == 0)
		return (1);
	while (num > 0)
	{
		num /= 10;
		i++;
	}
	return (i);
}

char	*ft_uitoa(unsigned int num)
{
	size_t	len;
	char	*str;

	len = int_len(num);
	str = (char *)malloc((len + 1));
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (num == 0)
	{
		str[0] = '0';
		return (str);
	}
	while (num != 0)
	{
		str[--len] = num % 10 + 48;
		num = num / 10;
	}
	return (str);
}

int	ft_unsigned(unsigned int num)
{
	char	*str;
	int		len;

	str = ft_uitoa(num);
	if (!str)
		return (-1);
	len = ft_putstr_fd(str, 1);
	free(str);
	if (len == -1)
		return (-1);
	return (len);
}
