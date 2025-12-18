/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:16:12 by musajid           #+#    #+#             */
/*   Updated: 2025/12/18 16:16:13 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Append a character to a dynamically allocated buffer in the arena
 * - Reallocates buffer with arena_realloc
 * - Updates len
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
 * Return exit code as string from shell in arena
 * - Used for $? expansion
 * - Returns string allocated in arena or NULL on failure
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
 * Return the value of an environment variable
 * - If key is NULL, returns empty string
 * - If key is "?", returns exit code string
 * - Allocates result in arena
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
 * Extract environment variable key from string at index i
 * - Updates i to point after the key
 * - Allocates key in arena
 * - Returns NULL on failure
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
 * Expand variable from string starting at index i
 * - Uses extract_env_key to get key
 * - Returns value from environment or special $? expansion
 * - Allocates result in arena
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
