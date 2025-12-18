#include "minishell.h"

/* 
 * Create a single environment node from a "KEY=VALUE" string
 * - Allocates key and value with ft_substr / ft_strdup
 * - Returns newly allocated t_env node or NULL on failure
 */
static t_env	*create_env_node(char *env_str)
{
	t_env	*node;
	char	*eq;

	eq = ft_strchr(env_str, '=');
	if (!eq)
		return (NULL);
	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->next = NULL;
	node->key = ft_substr(env_str, 0, eq - env_str);
	node->value = ft_strdup(eq + 1);
	if (!node->key || !node->value)
		return (free_env_node(node), NULL);
	return (node);
}

/* 
 * Initialize environment linked list from envp array
 * - Copies each "KEY=VALUE" string into t_env nodes
 * - Returns head of list or NULL on failure (frees partial list)
 */
t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*last;
	t_env	*node;
	int		i;

	head = NULL;
	last = NULL;
	i = 0;
	while (envp && envp[i])
	{
		node = create_env_node(envp[i]);
		if (!node)
			return (free_env(head), NULL);
		if (!head)
			head = node;
		else
			last->next = node;
		last = node;
		i++;
	}
	return (head);
}

/* 
 * Free the environment linked list
 * - Frees key, value, and node memory
 */
void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		if (env->key)
			free(env->key);
		if (env->value)
			free(env->value);
		free(env);
		env = tmp;
	}
}
