/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 16:02:03 by achowdhu          #+#    #+#             */
/*   Updated: 2025/10/04 19:56:45 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Main read–eval–print loop of minishell
 * - reads input
 * - tokenizes input
 * - prints tokens (debug)
 * - frees memory in all cases to avoid leaks
 */
void	repl_loop(t_shell *shell)
{
	char	*input;
	t_list	*tokens;

	while (shell->running)
	{
		input = read_input();
		if (!input) /* EOF (Ctrl-D) */
			break;

		if (*input)
		{
			/* Tokenize the input; tokenize returns NULL on error */
			tokens = tokenize(input);
			if (!tokens)
			{
				/* Tokenization failed (unmatched quote or malloc fail) */
				/* We skip execution and continue; input must be freed */
				free(input);
				continue;
			}

			/* Debug: print tokens */
			for (t_list *tmp = tokens; tmp; tmp = tmp->next)
				printf("TOKEN: %s\n", (char *)tmp->content);

			/* Free token list after processing */
			ft_lstclear(&tokens, free);
		}
		free(input);
	}
}
