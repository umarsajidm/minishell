/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ant-main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@hive.student.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 17:43:23 by achowdhu          #+#    #+#             */
/*   Updated: 2025/10/27 13:34:48 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	t_list	*tokens;
	t_list	*tmp;
	char	*input;

	(void)argc;
	(void)argv;
	shell.env = init_env(envp); //where is this defined
	shell.exit_code = 0;
	shell.running = true;

	setup_signals();

	while (shell.running)
	{
		input = read_input();
		if (!input)
			break;
		if (*input)
		{
			add_history(input); //why this add_history. i don't think it is needed here
			tokens = tokenize(input);
			tmp = tokens;
			while (tmp)
			{
				printf("TOKEN: %s\n", (char *)tmp->content);
				tmp = tmp->next;
			}
			ft_lstclear(&tokens, free);
		}
		free(input);
	}
	free_env(shell.env);
	return (shell.exit_code);
}
