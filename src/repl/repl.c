/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 16:02:03 by achowdhu          #+#    #+#             */
/*   Updated: 2025/10/04 16:17:52 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	repl_loop(t_shell *shell)
{
	char	*input;
	t_list	*tokens;
	t_list	*tmp;

	while (shell->running)
	{
		/* Read a line of input from the user */
		input = read_input();
		if (!input)
			break;

		if (*input)
		{
			/* Add input to history */
			add_history(input);

			/* Tokenize the input line */
			tokens = tokenize(input);

			/* For debugging, print all tokens */
			tmp = tokens;
			while (tmp)
			{
				printf("TOKEN: %s\n", (char *)tmp->content);
				tmp = tmp->next;
			}

			/* Free token list after processing */
			ft_lstclear(&tokens, free);
		}

		/* Free input line */
		free(input);
	}
}
