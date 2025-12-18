/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 14:34:45 by achowdhu          #+#    #+#             */
/*   Updated: 2025/12/18 14:34:46 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Count number of strings in argv array
 * - Returns 0 if argv is NULL
 */
int	count_argv(char **argv)
{
	int	i;

	i = 0;
	if (!argv)
		return (0);
	while (argv[i])
		i++;
	return (i);
}

/* 
 * Print parse error message
 * - If tok is provided, format msg with tok
 * - Returns NULL for convenience
 */
t_cmd	*parse_error(const char *msg, const char *tok)
{
	if (tok)
		ft_printf(msg, tok);
	else
		ft_printf("%s\n", msg);
	return (NULL);
}

/* 
 * Return length of variable name for $VAR
 * - Stops at first non-alnum/underscore
 */
int	iterate_key(const char *str)
{
	int	i;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}

/* 
 * Return 1 if string is NULL or contains only whitespace
 */
int	is_blank_line(const char *s)
{
	if (!s)
		return (1);
	while (*s && ft_isspace((unsigned char)*s))
		s++;
	return (*s == '\0');
}
