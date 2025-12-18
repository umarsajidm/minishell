/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 14:48:40 by achowdhu          #+#    #+#             */
/*   Updated: 2025/12/18 14:48:41 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Process a single token at the current input index
 * - Calls operator or word handler as needed
 * - Updates index `i` to next position
 * - Returns 1 on success, 0 on failure
 */
int	process_token(char *input, int *i, t_list **tokens, t_arena **arena)
{
	int	ret;

	ret = -1;
	if (is_operator_char(input[*i]))
		ret = handle_operator(input, *i, tokens, arena);
	else
		ret = handle_word(input, *i, tokens, arena);
	if (ret == -1)
		return (0);
	*i = ret;
	return (1);
}

/* 
 * Tokenize the input string into a linked list of tokens
 * - Allocates all tokens in arena memory
 * - Returns the head of the token list or NULL on failure
 */
t_list	*tokenize(char *input, t_arena **arena)
{
	t_list	*tokens;
	int		i;

	if (!input)
		return (NULL);
	tokens = NULL;
	i = 0;
	while (input[i])
	{
		i = skip_spaces(input, i);
		if (!input[i])
			break ;
		if (!process_token(input, &i, &tokens, arena))
			return (NULL);
	}
	return (tokens);
}
