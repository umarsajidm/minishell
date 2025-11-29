#include "minishell.h"

/* 
 * Create a single environment node from a "KEY=VALUE" string
 * - Returns newly allocated t_env node or NULL on failure
 */
static t_env	*create_env_node(char *env_str)
{
	t_env	*node;
	char	*eq;

	eq = ft_strchr(env_str, '=');                  // find '=' separator
	if (!eq)
		return (NULL);                             // invalid env string
	node = malloc(sizeof(t_env));                  // allocate node
	if (!node)
		return (NULL);                             // malloc failed
	node->key = ft_substr(env_str, 0, eq - env_str); // copy key
	node->value = ft_strdup(eq + 1);              // copy value
	node->next = NULL;                             // terminate list
	return (node);                                 // return node
}

/* 
 * Initialize environment linked list from envp array
 * - Copies each "KEY=VALUE" string into t_env nodes
 * - Returns head of list or NULL on failure
 */
t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*last;
	t_env	*node;
	int		i;

	head = NULL;                                   // head of list
	last = NULL;                                   // last node pointer
	i = 0;
	while (envp && envp[i])
	{
		node = create_env_node(envp[i]);          // create node
		if (node)
		{
			if (!head)
				head = node;                      // first node becomes head
			else
				last->next = node;                // append to end
			last = node;                          // update last
		}
		i++;
	}
	return (head);                                // return head of list
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
		tmp = env->next;                          // store next node
		if (env->key)
			free(env->key);                       // free key string
		if (env->value)
			free(env->value);                     // free value string
		free(env);                                // free node
		env = tmp;                                // move to next node
	}
}

/* 
 * Get value of an environment variable
 * - Returns pointer to value or NULL if not found
 */
char	*get_env_value(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)       // key matches
			return (env->value);                 // return value
		env = env->next;                          // move to next node
	}
	return (NULL);                               // key not found
}
