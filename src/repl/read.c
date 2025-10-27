/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 16:02:00 by achowdhu          #+#    #+#             */
/*   Updated: 2025/10/04 19:58:19 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Read a single line of input from the user and add it to history
 * Caller must free returned string. Returns NULL on EOF (Ctrl-D). */
char	*read_input(void)
{
	char	*line;

	line = readline("minishell$ "); // display prompt and read input
	if (!line)
		return (NULL);
	if (*line)
		add_history(line); // store non-empty lines in history
	return (line); // caller must free the returned string
}

/* Helper: append a line to existing heredoc content, safely handling memory.
 * Returns new content (malloc'd), or NULL on allocation failure. */
static char	*append_line(char *content, char *line)
{
	char	*tmp;
	char	*new_content;
	size_t	size;

	if (!content)
		return (strdup(line)); // first line, just duplicate
	tmp = content;
	/* +1 for '\n' and +1 for '\0' */
	size = strlen(content) + strlen(line) + 2;
	new_content = malloc(size);
	if (!new_content)
	{
		free(tmp); // free previous content to avoid leak
		return (NULL);
	}
	/* safe concatenation into allocated buffer */
	sprintf(new_content, "%s\n%s", tmp, line);
	free(tmp);
	return (new_content);
}

/* Read heredoc input until delimiter is matched.
 * Returns a malloc'd string with heredoc contents (including newlines),
 * or NULL on allocation failure. Caller must free the result. */
char	*read_heredoc(const char *delimiter)
{
	char	*line;
	char	*content = NULL;

	while (1)
	{
		line = readline("> "); // heredoc prompt
		if (!line || strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		content = append_line(content, line);
		free(line);
		if (!content)
			return (NULL); /* allocation failure, caller must handle NULL */
	}
	return (content); /* may be NULL (no lines read) or malloc'd content */
}
