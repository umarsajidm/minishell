/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@hive.student.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 22:38:08 by musajid           #+#    #+#             */
/*   Updated: 2025/12/18 22:38:21 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

char	*read_input(t_arena **arena)
{
	char	*line;
	char	*arena_line;

	line = NULL;
	if (isatty(STDIN_FILENO))
	{
		setup_parent_signals();
		line = readline("minishell$ ");
	}
	else
	{
		line = get_next_line(STDIN_FILENO);
		if (line && line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
	}
	if (!line)
		return (NULL);
	arena_line = arena_strdup(arena, line);
	free(line);
	return (arena_line);
}

static char	*append_heredoc_line(char *content, size_t *cur_len,
	char *line, t_arena **arena)
{
	size_t	len;

	len = ft_strlen(line);
	content = arena_realloc(arena, content, *cur_len, *cur_len + len + 2);
	if (!content)
		return (NULL);
	ft_memcpy(content + *cur_len, line, len);
	*cur_len += len;
	content[(*cur_len)++] = '\n';
	content[*cur_len] = '\0';
	return (content);
}

void	print_hd_err(const char *delimiter)
{
	ft_putstr_fd("minishell: warning: here-document at line ", 2);
	ft_putstr_fd("delimited by end-of-file (wanted `", 2);
	ft_putstr_fd((char *)delimiter, 2);
	ft_putstr_fd("')\n", 2);
}

static char	*read_heredoc_line(t_shell *shell, const char *delimiter)
{
	char	*line;

	setup_hd_signals();
	line = readline("> ");
	setup_parent_signals();
	if (g_signal != 0)
	{
		shell->exit_code = 128 + g_signal;
		if (line)
			free(line);
		return (NULL);
	}
	if (!line)
		return (print_hd_err(delimiter), NULL);
	return (line);
}

char	*read_heredoc(t_shell *shell, const char *delimiter)
{
	char	*line;
	char	*content;
	size_t	cur_len;

	if (!delimiter)
		return (NULL);
	content = arena_alloc(&shell->arena, 1);
	if (!content)
		return (NULL);
	content[0] = '\0';
	cur_len = 0;
	while (1)
	{
		line = read_heredoc_line(shell, delimiter);
		if (!line || ft_strcmp(line, delimiter) == 0)
			break ;
		content = append_heredoc_line(content, &cur_len, line, &shell->arena);
		if (!content)
			return (NULL);
	}
	return (content);
}
