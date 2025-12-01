#include "minishell.h"

/* 
 * Count number of strings in argv array
 * - Returns 0 if argv is NULL
 */
int	count_argv(char **argv)
{
	int	i;

	i = 0;
	while (argv && argv[i])
		i++;
	return (i);                                 // number of argv elements
}

/* 
 * Print parse error message
 * - If tok is provided, format msg with tok
 * - Returns NULL for convenience
 */
t_cmd	*parse_error(const char *msg, const char *tok)
{
	if (tok)
		ft_printf(msg, tok);                    // formatted error
	else
		ft_printf("%s\n", msg);                 // plain error
	return (NULL);                               // return NULL
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
	return (i);                                 // length of key
}

/* return 1 if string is NULL or contains only whitespace */
int is_blank_line(const char *s)
{
	if (!s)
		return (1);
	while (*s)
	{
		if (!(*s == ' ' || *s == '\t' || *s == '\n'
				|| *s == '\r' || *s == '\f' || *s == '\v'))
			return (0);
		s++;
	}
	return (1);
}
