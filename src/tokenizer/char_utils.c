/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:01:11 by achowdhu          #+#    #+#             */
/*   Updated: 2025/09/30 15:12:53 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Check if char is an operator | < > */
int is_operator_char(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

/* Duplicate a substring from start to end */
char *dup_word(const char *str, int start, int end)
{
    char *word;
    int len = end - start;
    int i = 0;

    word = malloc(len + 1);
    if (!word)
        return (NULL);
    while (start < end)
        word[i++] = str[start++];
    word[i] = '\0';
    return (word);
}

/* Skip spaces and tabs */
int skip_spaces(char *s, int i)
{
    while (s[i] && (s[i] == ' ' || s[i] == '\t'))
        i++;
    return (i);
}
