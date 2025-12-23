/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtarvain <jtarvain@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 13:10:14 by jtarvain          #+#    #+#             */
/*   Updated: 2025/12/18 15:23:41 by jtarvain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*alloc_node(const char *str);
static size_t	env_strlen(const char *str);

/*
** Updates the value of an existing environment variable.
** Finds the node matching the key in 'str'.
** If 'str' contains '=', updates the value.
** Frees the old value and allocates the new one.
** Returns 1 on success, 0 on malloc failure.
*/
int	update_env_node(const char *str, t_shell *shell)
{
	t_env	*match;
	char	*value;

	match = find_env_node(str, shell->env);
	value = ft_strchr(str, '=');
	if (!value)
		return (1);
	value++;
	if (match->value)
	{
		free(match->value);
		match->value = NULL;
	}
	match->value = ft_strdup(value);
	if (!match->value)
		return (0);
	return (1);
}

/*
** Adds a new environment variable node to the end of the list.
** Allocates the new node using alloc_node().
** Returns 1 on success, 0 on malloc failure.
*/
int	add_env_node(const char *str, t_shell *shell)
{
	t_env	*current;

	current = shell->env;
	if (!current)
	{
		shell->env = alloc_node(str);
		if (!shell->env)
			return (0);
		current = shell->env;
	}
	while (current->next)
		current = current->next;
	current->next = alloc_node(str);
	if (!current->next)
		return (0);
	return (1);
}

/*
** Allocates and initializes a new environment node.
** Parses 'str' to extract key and value.
** Key is substring up to '=' or end of string.
** Value is substring after '=', or NULL if no '='.
** Returns pointer to new node or NULL on malloc failure.
*/
static t_env	*alloc_node(const char *str)
{
	t_env	*new;
	char	*value;

	new = (t_env *)ft_calloc(1, sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_substr(str, 0, env_strlen(str));
	if (!new->key)
		return (free(new), NULL);
	value = ft_strchr(str, '=');
	if (!value)
		return (new);
	value++;
	if (!*value)
	{
		new->value = ft_strdup("");
		return (new);
	}
	new->value = ft_strdup(value);
	if (!new->value)
		return (free_env_node(new), NULL);
	return (new);
}

/*
** Calculates the length of the environment variable key.
** Scans until it hits '=' or null terminator.
** Returns the length.
*/
static size_t	env_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0' && str[i] != '=')
	{
		i++;
	}
	return (i);
}
