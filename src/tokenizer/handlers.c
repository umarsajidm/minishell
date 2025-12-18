/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 14:51:36 by achowdhu          #+#    #+#             */
/*   Updated: 2025/12/18 14:51:58 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Handle operators: |, <, >, <<, >>
 * - Creates a token node for the operator
 * - Returns next index after operator, -1 on failure
 */
int	handle_operator(char *s, int i, t_list **tokens, t_arena **arena)
{
	int		len;
	char	*op_str;

	len = 1;
	if ((s[i] == '<' && s[i + 1] == '<')
		|| (s[i] == '>' && s[i + 1] == '>'))
		len = 2;
	op_str = dup_word(arena, s, i, i + len);
	if (!op_str)
		return (-1);
	if (!create_token_node(arena, tokens, op_str, T_OPERATOR))
		return (-1);
	return (i + len);
}

/* 
 * Find the end index of a word in input string
 * - Considers quotes and operators
 * - Returns index of end, or -1 if unmatched quote
 */
static int	find_word_end(char *s, int i)
{
	char	quote;

	quote = 0;
	while (s[i])
	{
		if (quote)
		{
			if (s[i] == quote)
				quote = 0;
		}
		else
		{
			if (s[i] == '\'' || s[i] == '"')
				quote = s[i];
			else if (is_operator_char(s[i]) || ft_isspace(s[i]))
				break ;
		}
		i++;
	}
	if (quote)
		return (-1);
	return (i);
}

/* 
 * Handle a word token in input string
 * - Finds word boundaries and creates token node
 * - Returns index after word, -1 on failure
 */
int	handle_word(char *s, int i, t_list **tokens, t_arena **arena)
{
	int		start;
	char	*sub;
	int		end;

	start = i;
	end = find_word_end(s, i);
	if (end == -1)
	{
		ft_printf("minishell: syntax error: unmatched quote\n");
		return (-1);
	}
	sub = dup_word(arena, s, start, end);
	if (!sub || !create_token_node(arena, tokens, sub, T_WORD))
		return (-1);
	return (end);
}
