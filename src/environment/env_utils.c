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
			ft_printf("%s=%s\n", env->key, env->value); // print key=value
		env = env->next;                                 // move to next node
	}
}

/* 
 * Append a new environment variable to the shell
 * - Allocates a new t_env node and sets key/value
 * - Returns 1 on success, 0 on failure
 */
static int	append_env_var(t_shell *shell, const char *key, const char *value)
{
	t_env	*cur;
	t_env	*iter;

	cur = malloc(sizeof(t_env));                        // allocate node
	if (!cur)
		return (0);                                     // malloc failed
	cur->key = ft_strdup(key);                          // copy key
	cur->value = ft_strdup(value);                      // copy value
	cur->next = NULL;                                   // terminate node

	if (!shell->env)
	{
		shell->env = cur;                               // first node
		return (1);
	}
	iter = shell->env;
	while (iter->next)
		iter = iter->next;
	iter->next = cur;                                   // append to end
	return (1);                                         // success
}

/* 
 * Set or update an environment variable
 * - If key exists, updates value
 * - If key doesn't exist, appends new node
 */
int	set_env_var(t_shell *shell, const char *key, const char *value)
{
	t_env	*cur;

	if (!shell || !key || !value)
		return (0);                                     // invalid input

	cur = shell->env;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)            // key exists
		{
			free(cur->value);                          // free old value
			cur->value = ft_strdup(value);             // set new value
			return (cur->value != NULL);               // return success
		}
		cur = cur->next;                                // move to next node
	}
	return (append_env_var(shell, key, value));       // append new variable
}
