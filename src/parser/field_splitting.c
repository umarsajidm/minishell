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
		{
			in_word = 0;
		}
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

char	**field_split(const char *str, t_arena **arena)
{
	char		**result;
	int			word_count;
	int			i;
	const char	*word_start;

	if (!str)
		return (NULL);
	result = allocate_result_array(str, arena, &word_count);
	if (!result)
		return (NULL);
	i = 0;
	while (*str && i < word_count)
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
				return (NULL);
			i++;
		}
	}
	result[i] = NULL;
	return (result);
}
