/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@hive.student.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 17:43:23 by achowdhu          #+#    #+#             */
/*   Updated: 2025/10/30 13:54:02 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char **copy_envp(char **envp)
{
	char **env;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	env = malloc(sizeof(char *) *(i + 1));
	if (!env)
		return NULL;
	i = 0;
	while(envp[i])
	{
		env[i] = ft_strdup(envp[i]);
		i++;
	}
	env[i] = NULL;
	return (env);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	char **env;

	env = copy_envp(envp);

	// Initialize environment and shell status
  //shell.env = init_env(envp);
	shell.exit_code = 0;
	shell.running = true;

	// Setup signal handlers
	setup_signals();

	// Start the REPL loop
	repl_loop(&shell, env);

	// Free environment before exiting
	//free_env(shell.env);
	free(env);
	return (shell.exit_code);
}
