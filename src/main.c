/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@hive.student.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 18:10:37 by musajid           #+#    #+#             */
/*   Updated: 2025/10/17 19:23:48 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **copy_envp(char **envp)
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

static void	child_process(char **command, char **env)
{
	pid_t	pid;
	int status;

	pid = fork();
	if (pid == 0)
	{
		execution(command, env);
		perror("Minishell$ ");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
		waitpid(pid, &status, 0);
	else
		perror("fork");
}

//so readline prints the string that we passed and return what user types
int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	char **env;

	env = copy_envp(envp);
	while (1)
	{
		char *prompt = readline("Minishell$ ");
		if (!prompt)
			break;
		if (prompt)
			add_history(prompt);
		char **command = ft_split(prompt, ' ');
		if (!command || !command[0])
		{
			freearray(command);
			free(prompt);
			continue;
		}
		if (!(is_builtin(command, env)))
		{
			child_process(command, env);//need to check whether it is redirection or command and use fun accordingly.
			freearray(command);
			free(prompt);
		}
	}
	freearray(env);
	return 0;
}

//need to handle tabs and spaces



