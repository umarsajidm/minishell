#include "minishell.h"

/* * Append character to dynamically growing arena string */
char	*append_char(char *buf, size_t *len, char c, t_arena **arena)
{
	char	*tmp;

	tmp = arena_realloc(arena, buf, *len, *len + 2); // +1 for char, +1 for \0
	if (!tmp)
		return (NULL); // alloc failed
	tmp[*len] = c; // add character
	tmp[*len + 1] = '\0'; // null-terminate
	(*len)++; // update length
	return (tmp);
}

/* * Lookup key in shell environment or $? for expansion */
char	*expand_env_value(const char *key, t_shell *shell, t_arena **arena)
{
	t_env	*cur;
	char	*tmp;

	if (!key)
		return (arena_strdup(arena, "")); // safety check
	if (key[0] == '?')
	{
		tmp = ft_itoa(shell->exit_code); // get exit code string
		if (!tmp)
			return (NULL); // malloc failed
		cur = (t_env *)arena_strdup(arena, tmp); // copy to arena
		free(tmp); // free standard malloc
		return ((char *)cur); // return result
	}
	cur = shell->env;
	while (cur)
	{
		if (ft_strncmp(cur->key, key, ft_strlen(key) + 1) == 0)
			return (arena_strdup(arena, cur->value)); // return value
		cur = cur->next; // next env node
	}
	return (arena_strdup(arena, "")); // not found -> empty
}

/* * Helper to extract key for expand_variable */
static char	*extract_env_key(const char *str, size_t *i, t_arena **arena)
{
	size_t	start;
	char	*key;
	size_t	len;

	start = *i; // record start of key
	if (str[*i] == '?')
		(*i)++; // skip '?'
	else if (ft_isalpha(str[*i]) || str[*i] == '_')
	{
		while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
			(*i)++; // consume valid chars
	}
	else
	{
		len = 0;
		if (str[*i] == '$')
			(*i)++; /* consume repeated '$' to avoid infinite loop */
		return (append_char(NULL, &len, '$', arena));
	}
	key = arena_alloc(arena, *i - start + 1); // allocate key buffer
	if (!key)
		return (NULL);
	ft_strlcpy(key, str + start, *i - start + 1); // copy key
	return (key);
}

/* * Identify and expand a single variable */
char	*expand_variable(const char *str, size_t *i, t_shell *shell,
		t_arena **arena)
{
	char		*key;
	const char	*val;
	char		*res;
	size_t		len;
	size_t		j;

	key = extract_env_key(str, i, arena); // get variable name
	if (!key)
		return (NULL);
	if (key[0] == '$' && key[1] == '\0')
		return (key); // literal $
	val = expand_env_value(key, shell, arena); // get env value
	res = NULL;
	len = 0;
	j = 0;
	while (val && val[j])
	{
		res = append_char(res, &len, val[j], arena); // append value chars
		j++;
	}
	return (res);
}
