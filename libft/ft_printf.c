/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 09:07:10 by musajid           #+#    #+#             */
/*   Updated: 2025/08/06 13:16:12 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printf(const char	*format, ...);

static int	format_specifier(va_list args, const char format)
{
	int	count;
	int	ret;

	count = 0;
	ret = 0;
	if (format == 'c')
		ret = ft_putchar_fd(va_arg(args, int), 1);
	else if (format == 's')
		ret = ft_putstr_fd(va_arg(args, char *), 1);
	else if (format == 'd' || format == 'i')
		ret = ft_putnbr_fd(va_arg(args, int), 1);
	else if (format == 'u')
		ret = ft_unsigned(va_arg(args, unsigned int));
	else if (format == 'X')
		ret = ft_print_hex(va_arg(args, unsigned int), 1);
	else if (format == 'x')
		ret = ft_print_hex(va_arg(args, unsigned int), 0);
	else if (format == 'p')
		ret = ft_pointer(va_arg(args, void *));
	else if (format == '%')
		ret = ft_putchar_fd('%', 1);
	if (ret == -1)
		return (-1);
	count += ret;
	return (count);
}

static int	validation(const char *format, va_list arg)
{
	t_data	user;

	user.i = 0;
	user.count = 0;
	user.ret = 0;
	while (format[user.i])
	{
		if (format[user.i] == '%' && format[user.i + 1])
		{
			if (!ft_strchr("cspdiuxX%", format[user.i + 1]))
				return (write(1, "\n ERROR:invalid conversion format \n", 35));
			user.ret = format_specifier(arg, format[user.i + 1]);
			if (user.ret == -1)
				return (-1);
			user.count += user.ret;
			user.i += 2;
			continue ;
		}
		user.ret = ft_putchar_fd(format[user.i], 1);
		if (user.ret == -1)
			return (user.ret);
		user.count += user.ret;
		user.i++;
	}
	return (user.count);
}

int	ft_printf(const char *format, ...)
{
	va_list	arg;
	t_data	user;

	user.i = 0;
	user.ret = 0;
	user.count = 0;
	if (format == NULL)
		return (-1);
	va_start(arg, format);
	user.ret = validation(format, arg);
	if (user.ret == -1)
	{
		va_end(arg);
		return (-1);
	}
	user.count += user.ret;
	va_end(arg);
	return (user.count);
}
