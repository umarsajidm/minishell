#include "minishell.h"

/* * Lookup key in shell environment or $? for expansion
 * - Returns allocated string from arena
 */
static char	*expand_env_value(const char *key, t_shell *shell,
	t_arena **arena)
{
	t_env	*cur;

	if (!key)
		return (arena_strdup(arena, ""));             // safety check
	if (key[0] == '?')
	{
		char	*tmp;
		char	*res;

		tmp = ft_itoa(shell->exit_code);            // get exit code string
		if (!tmp)
			return (NULL);                           // malloc failed
		res = arena_strdup(arena, tmp);             // copy to arena
		free(tmp);                                  // free standard malloc
		return (res);                               // return result
	}
	cur = shell->env;
	while (cur)
	{
		if (ft_strncmp(cur->key, key, ft_strlen(key)) == 0
			&& cur->key[ft_strlen(key)] == '\0')
		{
			if (cur->value)
				return (arena_strdup(arena, cur->value)); // return value
			else
				return (arena_strdup(arena, ""));         // empty var
		}
		cur = cur->next;                              // next env node
	}
	return (arena_strdup(arena, ""));                 // not found -> empty
}

/* * Append character to dynamically growing arena string
 * - buf: current string (can be NULL)
 * - len: current length
 * - c: char to append
 */
static char	*append_char(char *buf, size_t *len, char c, t_arena **arena)
{
	char	*tmp;

	tmp = arena_realloc(arena, buf, *len, *len + 2); // +1 for char, +1 for \0
	if (!tmp)
		return (NULL);                                // alloc failed
	tmp[*len] = c;                                   // add character
	tmp[*len + 1] = '\0';                            // null-terminate
	(*len)++;                                        // update length
	return (tmp);
}

/* * Identify and expand a single variable
 * - Called when $ is found
 * - Moves index i past the variable name
 */
static char	*expand_variable(const char *str, size_t *i, t_shell *shell,
	t_arena **arena)
{
	size_t		start;
	size_t		j;
	char		*key;
	const char	*val;
	char		*res;
	size_t		len;

	start = *i;                                      // record start of key
	if (str[*i] == '?')
		(*i)++;                                      // skip '?'
	else if (ft_isalpha(str[*i]) || str[*i] == '_')
	{
		while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
			(*i)++;                                  // consume valid chars
	}
	else
	{
		len = 0;
		return (append_char(NULL, &len, '$', arena));
	}
	key = arena_alloc(arena, *i - start + 1);        // allocate key buffer
	ft_strlcpy(key, str + start, *i - start + 1);    // copy key
	val = expand_env_value(key, shell, arena);       // get env value
	res = NULL;
	len = 0;
	j = 0;
	while (val[j])
		res = append_char(res, &len, val[j++], arena); // append value chars
	return (res);
}

/* * Main Expansion Logic
 * - Removes quotes but respects their meaning
 * - Expands variables unless in single quotes
 */
char	*expand_string(const char *str, t_shell *shell, t_arena **arena)
{
	char	*res;
	size_t	len;
	size_t	i;
	char	*tmp;
	size_t	j;
	int		quote_state;                              // 0=None, 1=Single, 2=Double

	res = NULL;
	len = 0;
	i = 0;
	quote_state = 0;
	while (str[i])
	{
		/* CASE 1: Single Quote (') */
		if (str[i] == '\'')
		{
			if (quote_state == 2)                     // inside double quotes
				res = append_char(res, &len, str[i], arena);
			else                                      // toggle single quote state
				quote_state = (quote_state == 1) ? 0 : 1;
			i++;                                      // skip the quote itself (remove)
		}
		/* CASE 2: Double Quote (") */
		else if (str[i] == '"')
		{
			if (quote_state == 1)                     // inside single quotes
				res = append_char(res, &len, str[i], arena);
			else                                      // toggle double quote state
				quote_state = (quote_state == 2) ? 0 : 2;
			i++;                                      // skip the quote itself (remove)
		}
		/* CASE 3: Variable ($) - Only if NOT in single quotes */
		else if (str[i] == '$' && quote_state != 1)
		{
			i++;                                      // skip '$' sign
			tmp = expand_variable(str, &i, shell, arena);
			if (!tmp)
				return (NULL);                        // alloc fail
			j = 0;
			while (tmp[j])
				res = append_char(res, &len, tmp[j++], arena); // append result
		}
		/* CASE 4: Normal Character */
		else
		{
			res = append_char(res, &len, str[i], arena); // copy literal
			i++;                                      // next char
		}
	}
	return (res);
}

/* * Expand all arguments in the command list
 * - Modifies argv in-place
 */
int	expand_command_argv(t_cmd *cmd, t_shell *shell, t_arena **arena)
{
	int		i;
	char	*expanded;

	if (!cmd || !cmd->argv)
		return (1);                                   // nothing to do
	i = 0;
	while (cmd->argv[i])
	{
		expanded = expand_string(cmd->argv[i], shell, arena);
		if (!expanded)
			return (0);                               // expansion error
		cmd->argv[i] = expanded;                      // update argv
		i++;
	}
	return (1);                                       // success
}
