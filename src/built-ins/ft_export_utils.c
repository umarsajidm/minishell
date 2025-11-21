#include "minishell.h"

static t_env	*alloc_node(const char *str);
static size_t	env_strlen(const char *str);

int	update_env_node(const char *str, t_shell *shell)
{
	t_env	*match;
	char	*value;

	match = find_env_node(str, shell->env);
	value = ft_strchr(str, '=');
	value++;
	if (!*value)
		return (1); 
	if (match->value)
	{
		free(match->value);
		match->value = NULL;
	}
	match->value = ft_strdup(value);
	if (!match->value)
			return (1);
	return (0);
}

int	add_env_node(const char *str, t_shell *shell)
{
	if (!shell->env)
	{
		shell->env = alloc_node(str);
		if (!shell->env)
		{
			perror("add_env_node");
			return (1);
		}
	}
	while (shell->env->next)
		shell->env = shell->env->next;
	shell->env->next = alloc_node(str);
	if (!shell->env->next)
	{
		perror("alloc_node fail");
		return (1);
	}
	return (0);
}

static t_env	*alloc_node(const char *str)
{
	t_env	*new;
	char	*value;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->next = NULL;
	value = ft_strchr(str, '=');
	if (!value)
	{
		new->key = ft_substr(str, 0, env_strlen(str));
		if (!new->key)
			return (free(new), NULL);
	}
	new->key = ft_substr(str, 0, env_strlen(str));
	if (!new->key)
		return (free(new), NULL);
	value++;
	if (!*value)
	{
		new->value = ft_strdup("");
		return (new);
	}
	new->value = ft_strdup(value);
	if (!new->value)
	{
		free(new->key);
		free(new);
		return (NULL);
	}
	return (new);
}

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


wasd=