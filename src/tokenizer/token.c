/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@hive.student.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 13:57:14 by achowdhu          #+#    #+#             */
/*   Updated: 2025/10/25 15:18:50 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Tokenize the input string into a linked list of tokens.
 * Handles operators, quoted strings, and normal words.
 */
t_list *tokenize(char *input)
{
    t_list *tokens = NULL;  // Head of the token list
    int i = 0;              // Current index in input string

    while (input[i])
    {
        // Skip spaces and tabs
        i = skip_spaces(input, i);
        if (!input[i])  // End of string
            break;

        // Handle operators: |, <, >, <<, >>
        if (is_operator_char(input[i]))
            i = handle_operator(input, i, &tokens);

        // Handle quoted strings: 'single' or "double"
        else if (input[i] == '\'' || input[i] == '"')
        {
            i = handle_quote(input, i, &tokens);
            if (i == -1)  // Unmatched quote error
                return (NULL);
        }

        // Handle normal words (non-operator, non-quote)
        else
            i = handle_word(input, i, &tokens);
    }

    return (tokens);
}
