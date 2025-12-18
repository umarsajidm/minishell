/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 14:51:18 by achowdhu          #+#    #+#             */
/*   Updated: 2025/12/18 14:51:19 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Return 1 if character is an operator: |, <, >
 * - Otherwise returns 0
 */
int	is_operator_char(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

/* 
 * Skip spaces and tabs in string starting at index i
 * - Returns index of first non-space character
 */
int	skip_spaces(char *s, int i)
{
	while (s[i] && (s[i] == ' ' || s[i] == '\t'))
		i++;
	return (i);
}

/* 
 * Duplicate a substring from start to end into arena memory
 * - Returns pointer inside arena or NULL on failure
 */
char	*dup_word(t_arena **arena, const char *str, int start, int end)
{
	char	*word;
	int		len;

	len = end - start;
	word = arena_alloc(arena, len + 1);
	if (!word)
		return (NULL);
	ft_memcpy(word, str + start, len);
	word[len] = '\0';
	return (word);
}

/* 
 * Append a node to the end of the token list
 */
static void	list_append(t_list **tokens, t_list *node)
{
	t_list	*tmp;

	if (!*tokens)
		*tokens = node;
	else
	{
		tmp = *tokens;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = node;
	}
}

/* 
 * Create a token node and add it to the tokens list
 * - Allocates t_token and t_list inside arena
 * - Returns 1 on success, 0 on failure
 */
int	create_token_node(t_arena **ar, t_list **toks,
			char *content, t_token_type type)
{
	t_token	*tok;
	t_list	*node;

	tok = arena_alloc(ar, sizeof(t_token));
	if (!tok)
		return (0);
	tok->token = content;
	tok->type = type;
	tok->next = NULL;
	node = arena_alloc(ar, sizeof(t_list));
	if (!node)
		return (0);
	node->content = tok;
	node->next = NULL;
	list_append(toks, node);
	return (1);
}
