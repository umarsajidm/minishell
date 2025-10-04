/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 13:57:14 by achowdhu          #+#    #+#             */
/*   Updated: 2025/10/04 16:18:02 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list *tokenize(char *input)
{
    t_list *tokens = NULL;
    int i = 0;

    while (input[i])
    {
        i = skip_spaces(input, i);
        if (!input[i])
            break;
        if (is_operator_char(input[i]))
            i = handle_operator(input, i, &tokens);
        else if (input[i] == '\'' || input[i] == '"')
        {
            i = handle_quote(input, i, &tokens);
            if (i == -1)
                return (NULL);
        }
        else
            i = handle_word(input, i, &tokens);
    }
    return (tokens);
}
