/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:01:14 by achowdhu          #+#    #+#             */
/*   Updated: 2025/10/04 16:46:23 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Handle operators in the input string.
 * Supports single operators (|, <, >) and double operators (<<, >>).
 * Adds the operator as a token to the tokens list.
 * Returns the next index to process after the operator.
 */
int handle_operator(char *s, int i, t_list **tokens)
{
    if ((s[i] == '<' && s[i + 1] == '<') || (s[i] == '>' && s[i + 1] == '>'))
    {
        ft_lstadd_back(tokens, ft_lstnew(dup_word(s, i, i + 2)));
        i += 2;  // Skip the double operator
    }
    else
    {
        ft_lstadd_back(tokens, ft_lstnew(dup_word(s, i, i + 1)));
        i++;      // Skip the single operator
    }
    return (i);
}

/* 
 * Handle quoted strings in the input.
 * Supports single quotes (') and double quotes (").
 * Everything inside quotes is treated as a single token.
 * Prints an error and returns -1 if the quote is unmatched.
 * Returns the next index to process after the closing quote.
 */
int handle_quote(char *s, int i, t_list **tokens)
{
    char quote = s[i];   // Store the quote character
    int start = ++i;     // Start of content inside the quotes

    while (s[i] && s[i] != quote)
        i++;

    if (!s[i])  // Unmatched quote
    {
        printf("minishell: syntax error: unmatched quote\n");
        return (-1);
    }

    ft_lstadd_back(tokens, ft_lstnew(dup_word(s, start, i)));
    return (i + 1); // Move past the closing quote
}

/* 
 * Handle normal words in the input.
 * A word ends at a space, tab, operator, or quote.
 * Adds the word as a token to the tokens list.
 * Returns the next index to process after the word.
 */
int handle_word(char *s, int i, t_list **tokens)
{
    int start = i;

    while (s[i] && !is_operator_char(s[i]) && s[i] != ' ' && s[i] != '\t'
           && s[i] != '\'' && s[i] != '"')
        i++;

    ft_lstadd_back(tokens, ft_lstnew(dup_word(s, start, i)));
    return (i);
}
