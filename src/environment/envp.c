/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:07:44 by musajid           #+#    #+#             */
/*   Updated: 2025/12/18 16:07:44 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	sizeoflist(t_env *env);
static char		*copyfromlisttoarr(t_env *env);

//making the envp from linked list of env for execve func
char	**envp_arr(t_shell *shell)
{
	t_env	*env;
	int		i;
	char	**envp;

	env = shell->env;
	envp = ft_calloc(sizeoflist(shell->env) + 1, sizeof(char *));
	if (!envp)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->value)
			envp[i++] = copyfromlisttoarr(env);
		env = env->next;
	}
	envp[i] = NULL;
	return (envp);
}

static size_t	sizeoflist(t_env *env)
{
	t_env	*cur;
	size_t	i;

	i = 0;
	cur = env;
	while (cur)
	{
		if (cur->value)
			i++;
		cur = cur->next;
	}
	return (i);
}

static char	*copyfromlisttoarr(t_env *env)
{
	size_t	keylen;
	size_t	valuelen;
	char	*dest;

	keylen = ft_strlen(env->key);
	valuelen = ft_strlen(env->value);
	dest = ft_calloc(keylen + valuelen + 2, sizeof(char));
	ft_memcpy(dest, env->key, keylen);
	dest[keylen] = '=';
	ft_memcpy(dest + keylen + 1, env->value, valuelen);
	dest[keylen + valuelen + 1] = '\0';
	return (dest);
}
