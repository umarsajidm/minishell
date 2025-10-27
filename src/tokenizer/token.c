/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 13:57:14 by achowdhu          #+#    #+#             */
/*   Updated: 2025/10/04 19:56:38 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Process one token based on current character.
 * Calls the proper handler and advances index `i`.
 * Returns 1 on success, 0 on error (malloc fail or unmatched quote).
 */
int	process_token(char *input, int *i, t_list **tokens)
{
	int	ret;

	if (is_operator_char(input[*i]))
		ret = handle_operator(input, *i, tokens);
	else if (input[*i] == '\'' || input[*i] == '"')
		ret = handle_quote(input, *i, tokens);
	else
		ret = handle_word(input, *i, tokens);

	if (ret == -1)
		return (0);
	*i = ret;
	return (1);
}

/* 
 * Tokenize the input string into a linked list of tokens.
 * Handles operators, quoted strings, and normal words.
 * On error (malloc failure / unmatched quote) frees any tokens created
 * and returns NULL.
 */
t_list	*tokenize(char *input)
{
	t_list	*tokens;
	int		i;

	if (!input)
		return (NULL);
	tokens = NULL;
	i = 0;
	while (input[i])
	{
		/* Skip whitespace */
		i = skip_spaces(input, i);
		if (!input[i])
			break;

		/* Process next token (operator, quote, or word) */
		if (!process_token(input, &i, &tokens))
		{
			ft_lstclear(&tokens, free);
			return (NULL);
		}
	}
	return (tokens);
}
