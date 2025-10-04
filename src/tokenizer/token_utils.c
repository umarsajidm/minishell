/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:01:14 by achowdhu          #+#    #+#             */
/*   Updated: 2025/09/30 15:12:51 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Handle operators like |, <, >>, << */
int handle_operator(char *s, int i, t_list **tokens)
{
    if ((s[i] == '<' && s[i + 1] == '<') || (s[i] == '>' && s[i + 1] == '>'))
    {
        ft_lstadd_back(tokens, ft_lstnew(dup_word(s, i, i + 2)));
        i += 2;
    }
    else
    {
        ft_lstadd_back(tokens, ft_lstnew(dup_word(s, i, i + 1)));
        i++;
    }
    return (i);
}

/* Handle quotes ' or " */
int handle_quote(char *s, int i, t_list **tokens)
{
    char quote = s[i];
    int start = ++i;

    while (s[i] && s[i] != quote)
        i++;
    if (!s[i])
    {
        printf("minishell: syntax error: unmatched quote\n");
        return (-1);
    }
    ft_lstadd_back(tokens, ft_lstnew(dup_word(s, start, i)));
    return (i + 1);
}

/* Handle normal words */
int handle_word(char *s, int i, t_list **tokens)
{
    int start = i;

    while (s[i] && !is_operator_char(s[i]) && s[i] != ' ' && s[i] != '\t'
           && s[i] != '\'' && s[i] != '"')
        i++;
    ft_lstadd_back(tokens, ft_lstnew(dup_word(s, start, i)));
    return (i);
}
