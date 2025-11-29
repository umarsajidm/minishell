#include "minishell.h"

/* 
 * Lookup key in shell environment or $? for expansion
 * - Uses env.c get_env_value for normal keys
 * - Allocates result in arena
 */
static char	*expand_env_value(const char *key, t_shell *shell, t_arena **arena)
{
	t_env	*cur;

	if (!key)
		return (arena_strdup(arena, ""));            // empty key -> empty string
	if (key[0] == '?')
	{
		char	*tmp;
		char	*res;

		tmp = ft_itoa(shell->exit_code);             // convert exit code to string
		if (!tmp)
			return (NULL);                           // malloc failed
		res = arena_strdup(arena, tmp);             // copy into arena
		free(tmp);                                  // free temporary string
		return (res);                               // return expanded value
	}
	cur = shell->env;
	while (cur)
	{
		if (!ft_strncmp(cur->key, key, ft_strlen(cur->key))
			&& cur->key[ft_strlen(cur->key)] == '\0') // exact match
		{
			if (cur->value)
				return (arena_strdup(arena, cur->value)); // value from env
			else
				return (arena_strdup(arena, ""));         // empty value
		}
		cur = cur->next;                                 // next env node
	}
	return (arena_strdup(arena, ""));                    // key not found
}

/* 
 * Append character to dynamically growing arena string
 * - buf may be NULL if first char
 * - len is current length of string
 * - reallocates arena buffer
 */
static char	*append_char(char *buf, size_t *len, char c, t_arena **arena)
{
	char	*tmp;

	tmp = arena_realloc(arena, buf, *len, *len + 2);    // +1 for char, +1 for '\0'
	if (!tmp)
		return (NULL);                                   // realloc failed
	tmp[*len] = c;                                      // add char
	tmp[*len + 1] = '\0';                               // null-terminate
	(*len)++;                                           // increment length
	return (tmp);
}

/* 
 * Expand a single variable $VAR or $? in a string
 * - str points at current parsing location
 * - i is updated past the variable name
 */
char	*expand_variable(const char *str, size_t *i,
	t_shell *shell, t_arena **arena)
{
	size_t		start;
	size_t		j;
	char		*key;
	const char	*val;
	char		*res;
	size_t		len;

	start = *i;
	if (str[*i] == '?')
		(*i)++;                                           // special variable '?'
	else if (ft_isalpha(str[*i]) || str[*i] == '_')
		*i += iterate_key(str + *i);                     // consume key chars
	else
	{
		len = 0;
		return (append_char(NULL, &len, '$', arena));    // invalid -> literal '$'
	}
	key = arena_alloc(arena, *i - start + 1);            // allocate key string
	ft_strlcpy(key, str + start, *i - start + 1);       // copy key from input
	val = expand_env_value(key, shell, arena);           // get value
	res = NULL;
	len = 0;
	j = 0;
	while (val[j])
	{
		res = append_char(res, &len, val[j], arena);     // append each char
		j++;
	}
	return (res);
}

/* 
 * Expand all $VAR or $? in a string using shell env and arena
 */
char	*expand_string(const char *str, t_shell *shell, t_arena **arena)
{
	char	*res;
	size_t	len;
	size_t	i;
	char	*tmp;
	size_t	j;

	res = NULL;
	len = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			i++;                                           // skip '$'
			tmp = expand_variable(str, &i, shell, arena); // expand variable
			if (!tmp)
				return (NULL);                            // expansion failed
			j = 0;
			while (tmp[j])
			{
				res = append_char(res, &len, tmp[j], arena); // append expanded chars
				j++;
			}
		}
		else
			res = append_char(res, &len, str[i++], arena);   // copy literal char
	}
	return (res);
}

/* 
 * Expand all argv words in a command
 * - Modifies cmd->argv in-place
 */
int	expand_command_argv(t_cmd *cmd, t_shell *shell, t_arena **arena)
{
	int		i;
	char	*expanded;

	if (!cmd || !cmd->argv)
		return (1);                                       // nothing to expand
	i = 0;
	while (cmd->argv[i])
	{
		expanded = expand_string(cmd->argv[i], shell, arena); // expand word
		if (!expanded)
			return (0);                                   // expansion failed
		cmd->argv[i] = expanded;                         // replace original
		i++;
	}
	return (1);                                           // success
}
