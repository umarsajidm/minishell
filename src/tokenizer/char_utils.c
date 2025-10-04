/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:01:11 by achowdhu          #+#    #+#             */
/*   Updated: 2025/10/04 16:45:48 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Check if a character is a shell operator.
 * Returns 1 if the character is |, <, or >, otherwise 0.
 */
int is_operator_char(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

/* 
 * Duplicate a substring from `start` to `end` (exclusive) of a string.
 * Allocates memory for the new string, which must be freed by the caller.
 */
char *dup_word(const char *str, int start, int end)
{
    char *word;
    int len = end - start;
    int i = 0;

    word = malloc(len + 1); // +1 for null terminator
    if (!word)
        return (NULL);

    while (start < end)
        word[i++] = str[start++];
    word[i] = '\0';  // Null-terminate the string

    return (word);
}

/* 
 * Skip spaces and tabs in a string starting from index `i`.
 * Returns the index of the first non-space character.
 */
int skip_spaces(char *s, int i)
{
    while (s[i] && (s[i] == ' ' || s[i] == '\t'))
        i++;
    return (i);
}
