/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_splitting.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:16:12 by achowdhu          #+#    #+#             */
/*   Updated: 2025/12/18 20:39:42 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static int	count_words(const char *s)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s == FIELD_SEP)
			in_word = 0;
		else if (!in_word)
		{
			in_word = 1;
			count++;
		}
		s++;
	}
	return (count);
}


static char	**allocate_result_array(const char *str, t_arena **arena,
				int *word_count)
{
	*word_count = count_words(str);
	return (arena_alloc(arena, sizeof(char *) * (*word_count + 1)));
}

/*
 * Fill the allocated array with words split by FIELD_SEP
 */
static void	fill_words(const char *str, char **result, int word_count,
		t_arena **arena)
{
	int			i;
	const char	*word_start;
	int			i;

	i = 0;
	while (*str)
	{
		while (*str && *str == FIELD_SEP)
			str++;
		if (*str)
		{
			word_start = str;
			while (*str && *str != FIELD_SEP)
				str++;
			result[i] = arena_strndup(arena, word_start, str - word_start);
			if (!result[i])
				return ;
			i++;
		}
	}
	result[i] = NULL;
}

/*
 * Split a string by FIELD_SEP into a NULL-terminated array
 */
char	**field_split(const char *str, t_arena **arena)
{
	char	**result;
	int		word_count;

	if (!str)
		return (NULL);
	result = allocate_result_array(str, arena, &word_count);
	if (!result)
		return (NULL);
	fill_words(str, result, word_count, arena);
	return (result);
}

/*
 * Handle field splitting after expansion and add words to argv
 */
int	handle_field_splitting(char *expanded, t_parser_state *p)
{
	char	**words;
	int		i;

	words = field_split(expanded, p->arena);
	if (!words)
		return (0);
	i = 0;
	while (words[i])
	{
		if (!ensure_current_cmd(p->cur, p->head, p->arena)
			|| !add_word_to_argv(*p->cur, words[i], p->arena))
			return (0);
		if (!(*p->cur)->argv && i == 0)
			(*p->cur)->unexpanded_cmd = p->tok->token;
		i++;
	}
	return (1);
}
