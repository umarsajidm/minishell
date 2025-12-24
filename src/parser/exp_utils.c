/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:16:12 by musajid           #+#    #+#             */
/*   Updated: 2025/12/18 21:12:18 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Append a single character to a dynamically growing buffer
 * @param buf The current buffer (can be NULL)
 * @param len Pointer to the current length of the buffer
 * @param c The character to append
 * @param arena The memory arena for reallocation
 * @return The updated buffer with the new character or NULL on failure
 *
 * - Reallocates buffer using arena
 * - Updates length and keeps string NULL-terminated
 * - Returns updated buffer or NULL on failure
 */
char	*append_char(char *buf, size_t *len, char c, t_arena **arena)
{
	char	*tmp;

	tmp = arena_realloc(arena, buf, *len, *len + 2);
	if (!tmp)
		return (NULL);
	tmp[*len] = c;
	tmp[*len + 1] = '\0';
	(*len)++;
	return (tmp);
}

/*
 * Expand special variable `$?`
 * @param shell The shell structure containing the last exit code
 * @param arena The memory arena
 * @return The string representation of the exit code or NULL
 *
 * - Converts last exit code to string
 * - Stores result in arena memory
 */
static char	*handle_exit_code_expansion(t_shell *shell, t_arena **arena)
{
	char	*tmp;
	char	*res;

	tmp = ft_itoa(shell->exit_code);
	if (!tmp)
		return (NULL);
	res = arena_strdup(arena, tmp);
	free(tmp);
	return (res);
}

/*
 * Retrieve the value of an environment variable
 * @param key The key of the variable to find
 * @param shell The shell structure
 * @param arena The memory arena
 * @return The value of the variable, empty string if not found, or NULL on error
 *
 * - Handles `$?` separately
 * - Returns empty string if variable exists but has no value
 * - Returns NULL if variable does not exist
 */
char	*expand_env_value(const char *key, t_shell *shell, t_arena **arena)
{
	t_env	*cur;

	if (!key)
		return (arena_strdup(arena, ""));
	if (key[0] == '?')
		return (handle_exit_code_expansion(shell, arena));
	cur = shell->env;
	while (cur)
	{
		if (ft_strncmp(cur->key, key, ft_strlen(key) + 1) == 0)
		{
			if (cur->value)
				return (arena_strdup(arena, cur->value));
			return (arena_strdup(arena, ""));
		}
		cur = cur->next;
	}
	return (NULL);
}

/*
 * Extract an environment variable key after `$`
 * @param str The input string
 * @param i Pointer to the current index in the string
 * @param arena The memory arena
 * @return The extracted key or a literal `$` string
 *
 * - Supports `$?`, valid identifiers, and literal `$`
 * - Advances parsing index accordingly
 */
static char	*extract_env_key(const char *str, size_t *i, t_arena **arena)
{
	size_t	start;
	char	*key;
	size_t	len;

	start = *i;
	if (str[*i] == '?')
		(*i)++;
	else if (ft_isalpha(str[*i]) || str[*i] == '_')
	{
		while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
			(*i)++;
	}
	else
	{
		len = 0;
		if (str[*i] == '$')
			(*i)++;
		return (append_char(NULL, &len, '$', arena));
	}
	key = arena_alloc(arena, *i - start + 1);
	if (!key)
		return (NULL);
	ft_strlcpy(key, str + start, *i - start + 1);
	return (key);
}

/*
 * Expand a variable starting at `$` in the input string
 * @param str The input string
 * @param i Pointer to the current index (at the `$` character)
 * @param shell The shell structure
 * @param arena The memory arena
 * @return The expanded value string or NULL on error
 *
 * - Extracts key and resolves its value
 * - Advances index past the variable name
 */
char	*expand_variable(const char *str, size_t *i, t_shell *shell,
		t_arena **arena)
{
	char		*key;
	const char	*val;

	key = extract_env_key(str, i, arena);
	if (!key)
		return (NULL);
	if (key[0] == '$' && key[1] == '\0')
		return (key);
	val = expand_env_value(key, shell, arena);
	if (!val)
		return (NULL);
	return ((char *)val);
}
