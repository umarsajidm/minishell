/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:01:14 by achowdhu          #+#    #+#             */
/*   Updated: 2025/10/04 19:56:40 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Handle operators in the input string.
 * Supports single operators (|, <, >) and double operators (<<, >>).
 * Adds the operator as a token to the tokens list.
 * Returns the next index to process after the operator, or -1 on error.
 */
int handle_operator(char *s, int i, t_list **tokens)
{
	char	*token;
	t_list	*node;
	int		len;

	if ((s[i] == '<' && s[i + 1] == '<') || (s[i] == '>' && s[i + 1] == '>'))
		len = 2;
	else
		len = 1;

	token = dup_word(s, i, i + len);
	if (!token)
		return (-1);
	node = ft_lstnew(token);
	if (!node)
	{
		free(token);
		return (-1);
	}
	ft_lstadd_back(tokens, node);
	return (i + len);
}

/* 
 * Handle quoted strings in the input.
 * Supports single quotes (') and double quotes (").
 * Everything inside quotes is treated as a single token.
 * Prints an error and returns -1 if the quote is unmatched or on allocation failure.
 */
int handle_quote(char *s, int i, t_list **tokens)
{
	char	quote;
	int		start;
	char	*word;
	t_list	*node;

	quote = s[i];
	i++;
	start = i;
	while (s[i] && s[i] != quote)
		i++;
	if (!s[i])  /* Unmatched quote */
	{
		printf("minishell: syntax error: unmatched quote\n");
		return (-1);
	}
	word = dup_word(s, start, i);
	if (!word)
		return (-1);
	node = ft_lstnew(word);
	if (!node)
	{
		free(word);
		return (-1);
	}
	ft_lstadd_back(tokens, node);
	return (i + 1); /* Move past closing quote */
}

/* 
 * Handle normal words in the input.
 * A word ends at a space, tab, operator, or quote.
 * Adds the word as a token to the tokens list.
 * Returns the next index to process after the word, or -1 on error.
 */
int handle_word(char *s, int i, t_list **tokens)
{
	int		start;
	char	*word;
	t_list	*node;

	start = i;
	while (s[i] && !is_operator_char(s[i]) && s[i] != ' ' && s[i] != '\t'
		&& s[i] != '\'' && s[i] != '"')
		i++;
	if (i == start)
		return (i); /* nothing to do */
	word = dup_word(s, start, i);
	if (!word)
		return (-1);
	node = ft_lstnew(word);
	if (!node)
	{
		free(word);
		return (-1);
	}
	ft_lstadd_back(tokens, node);
	return (i);
}
