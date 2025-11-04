#include "minishell.h"

/* 
 * Initialize environment linked list from envp array
 * - Copies each "KEY=VALUE" string into t_env nodes
 * - Returns head of list or NULL on failure
 */
t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*node;
	t_env	*last;
	char	*eq;
	int		i;

	head = NULL;
	last = NULL;
	i = 0;
	while (envp && envp[i])                        // iterate through envp array
	{
		eq = ft_strchr(envp[i], '=');             // find '=' separator
		if (!eq)
		{
			i++;
			continue;                              // skip invalid entries
		}

		node = malloc(sizeof(t_env));             // allocate new node
		if (!node)
			return (NULL);                        // malloc failed

		node->key = ft_substr(envp[i], 0, eq - envp[i]);  // copy key
		node->value = ft_strdup(eq + 1);                   // copy value
		node->next = NULL;

		if (!head)
			head = node;                           // first node becomes head
		else
			last->next = node;                     // append to end

		last = node;                               // update last pointer
		i++;
	}
	return (head);                                // return head of env list
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
		tmp = env->next;                         // store next pointer
		if (env->key)
			free(env->key);                      // free key string
		if (env->value)
			free(env->value);                    // free value string
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
		env = env->next;                          // move to next
	}
	return (NULL);                               // not found
}
