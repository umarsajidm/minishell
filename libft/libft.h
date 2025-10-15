/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 14:55:11 by INTRA_LOGIN       #+#    #+#             */
/*   Updated: 2025/07/26 17:08:00 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include <stdarg.h>

typedef struct s_data
{
	int	i;
	int	ret;
	int	count;
}	t_data;

void	*ft_bzero(void	*str, size_t n);
void	*ft_calloc(size_t nmemb, size_t size);
int		ft_isalnum(int a);
int		ft_isalpha(int a);
int		ft_isascii(int a);
int		ft_isdigit(int a);
int		ft_isprint(char c);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
void	*ft_memcpy(void	*dst, const void	*src, size_t n);
void	*ft_memmove(void	*dst, const void	*src, size_t	n);
void	*ft_memset(void	*str, int c, size_t n);
char	*ft_strdup(const char	*s);
size_t	ft_strlen(const char *c);
size_t	ft_strlcpy(char	*dst, const char	*src, size_t n);
size_t	ft_strlcat(char *dst, const char	*src, size_t sizeofdst);
char	*ft_strchr(const char	*str, int c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strrchr(const char *s, int c);
int		ft_atoi(const char	*str);
char	*ft_substr(char const	*s, unsigned int start, size_t	len);
char	*ft_strtrim(const char *s1, const char *set);
char	*ft_strjoin(char const *s1, char const *s2);
int		ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
int		ft_putnbr_fd(int n, int fd);
int		ft_putchar_fd(char c, int fd);
int		ft_tolower(int c);
int		ft_toupper(int c);
char	*ft_strmapi(const char *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
char	*ft_strnstr(const char *str, const char *to_find, size_t len);
char	*ft_itoa(int n);
char	**ft_split(char const *s, char c);
char	*ft_uitoa(unsigned int num);
int		ft_printf(const char	*format, ...);
int		ft_unsigned(unsigned int num);
int		ft_print_hex(unsigned long n, int is_uppercase);
int		ft_pointer(void *pointer);

#endif
