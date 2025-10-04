/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 16:02:00 by achowdhu          #+#    #+#             */
/*   Updated: 2025/10/04 16:17:50 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Read a single line of input from the user and add it to history */
char	*read_input(void)
{
	char	*line;

	line = readline("minishell$ "); // display prompt and read input
	if (line && *line)
		add_history(line); // store non-empty lines in history
	return line; // caller must free the returned string
}

/* Append a line to existing heredoc content, safely handling memory */
static char	*append_line(char *content, char *line)
{
	char	*tmp;
	char	*new_content;

	if (!content)
		return strdup(line); // first line, just duplicate
	tmp = content;
	new_content = malloc(strlen(content) + strlen(line) + 2); // +1 for \n, +1 for \0
	if (!new_content)
	{
		free(tmp); // free old content to avoid memory leak
		return NULL;
	}
	sprintf(new_content, "%s\n%s", tmp, line); // concatenate old + new line
	free(tmp); // free previous content
	return new_content; // caller must manage final memory
}

/* Read heredoc input until delimiter is matched
   Lines are appended into a single string with newlines.
   Returns NULL on allocation failure. Caller must free. */
char	*read_heredoc(const char *delimiter)
{
	char	*line;
	char	*content = NULL;

	while (1)
	{
		line = readline("> "); // heredoc prompt
		if (!line || strcmp(line, delimiter) == 0) // stop on EOF or delimiter
		{
			free(line);
			break;
		}
		content = append_line(content, line); // add line to heredoc content
		free(line); // free temporary line
		if (!content) // malloc failure
			return NULL;
	}
	return content; // final heredoc string
}
