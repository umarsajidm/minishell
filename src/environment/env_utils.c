#include "minishell.h"

/* 
 * Print all environment variables (for debug)
 * - Format: KEY=VALUE
 */
void	dbg_print_env(t_env *env)
{
	while (env)
	{
		if (env->key && env->value)
		{
			ft_printf("%s=%s\n", env->key, env->value); // print key=value
		}
		env = env->next;                              // move to next node
	}
}

/* 
 * Set or update an environment variable
 * - If key exists, updates value
 * - If key doesn't exist, appends new node
 * - Uses malloc for node and value
 */
int	set_env_var(t_shell *shell, const char *key, const char *value)
{
	t_env	*cur;

	if (!shell || !key || !value)
		return (0);                                  // invalid input

	cur = shell->env;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)         // key exists
		{
			free(cur->value);                       // free old value
			cur->value = ft_strdup(value);          // set new value
			return (cur->value != NULL);            // return success
		}
		cur = cur->next;                             // move to next
	}

	/* key not found -> append new node */
	cur = malloc(sizeof(t_env));
	if (!cur)
		return (0);                                  // malloc fail

	cur->key = ft_strdup(key);
	cur->value = ft_strdup(value);
	cur->next = NULL;

	if (!shell->env)
		shell->env = cur;                            // first node
	else
	{
		t_env *iter = shell->env;
		while (iter->next)
			iter = iter->next;
		iter->next = cur;                            // append
	}
	return (1);                                      // success
}
