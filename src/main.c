/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 18:10:37 by musajid           #+#    #+#             */
/*   Updated: 2025/10/15 19:05:57 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	(void)envp;


	char *prompt = readline("Minishell$ ");
	char **command = ft_split(prompt, ' ');

	pid_t pid = fork();
	if (pid == 0)
	execution(command[0], envp);

	return 0;
}

