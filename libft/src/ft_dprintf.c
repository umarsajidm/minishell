#include "libft.h"

static int	ft_dputhex(int fd, uintptr_t decimal, char conv, int i, int p)
{
	char		hexadecimal[20];
	const char	*hex_table;

	if (!decimal && conv == 'p')
		return (write(fd, "(nil)", 5));
	if (decimal == 0)
		return (write(fd, "0", 1));
	if (conv == 'p')
		p = ft_putstr_fd("0x", fd);
	if (conv == 'x' || conv == 'p')
		hex_table = "0123456789abcdef";
	else
		hex_table = "0123456789ABCDEF";
	while (decimal)
	{
		hexadecimal[i++] = hex_table[decimal & 0xF];
		decimal >>= 4;
	}
	hexadecimal[i] = '\0';
	ft_strrev(hexadecimal);
	ft_putstr_fd(hexadecimal, fd);
	return (i + p);
}

static int	ft_dwrite_conversion(int fd, va_list *args, char conv)
{
	if (conv == 'c')
		return (ft_putchar_fd((char)va_arg(*args, int), fd));
	if (conv == 's')
		return (ft_putstr_fd(va_arg(*args, char *), fd));
	if (conv == 'p')
		return (ft_dputhex(fd, (uintptr_t)va_arg(*args, void *), conv, 0, 0));
	if (conv == 'x' || conv == 'X')
		return (ft_dputhex(fd, (unsigned)va_arg(*args, unsigned), conv, 0, 0));
	if (conv == 'd' || conv == 'i')
		return (ft_putnbr_fd((int)va_arg(*args, int), fd));
	if (conv == 'u')
		return (ft_uputnbr_fd((unsigned)va_arg(*args, unsigned), fd));
	return (write(fd, "%", 1));
}

int	ft_dprintf(int fd, const char *format, ...)
{
	va_list	args;
	int		count;

	if (!format)
		return (-1);
	count = 0;
	while (format[count])
		if (format[count++] == '%')
			if (!format[count] || !ft_strchr("cspxXdiu%", format[count++]))
				return (-1);
	va_start(args, format);
	count = 0;
	while (*format)
	{
		if (*format == '%' && format[1] && ft_strchr("cspxXdiu%", format[1]))
			count += ft_dwrite_conversion(fd, &args, *++format);
		else
			count += write(fd, &*format, 1);
		format++;
	}
	return (va_end(args), count);
}
