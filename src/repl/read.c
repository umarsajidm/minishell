#include "minishell.h"
#include <unistd.h>

/* Read a single line of input from user and duplicate into arena
** - Adds non-empty lines to history
** - Returns pointer in arena memory
*/
char	*read_input(t_arena **arena)
{
	char	*line;				// line from readline

	if (isatty(STDIN_FILENO))
	{
		setup_parent_signals();
		line = readline("minishell$ ");	// display prompt and read input
	}
	else
	{
		line = get_next_line(STDIN_FILENO);
		if (line && line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
	}
	if (!line)
		return (NULL);			// Ctrl-D returns NULL

	char *arena_line = arena_strdup(arena, line);
	free(line);
	return (arena_line);
}

/* Append a single line to heredoc content stored in arena
** - Adds newline automatically
** - Returns updated content or NULL on failure
*/
static char	*append_heredoc_line(char *content, size_t *cur_len,
	char *line, t_arena **arena)
{
	size_t	len;				// length of the line

	len = ft_strlen(line);			// compute line length
	content = arena_realloc(arena, content, *cur_len, *cur_len + len + 2); // +\n+\0
	if (!content)
		return (NULL);			// realloc failed
	ft_memcpy(content + *cur_len, line, len);	// append line bytes
	*cur_len += len;			// advance cursor
	content[(*cur_len)++] = '\n';		// append newline
	content[*cur_len] = '\0';		// null terminate
	return (content);			// return updated content
}

/* Read a single heredoc line from user
** - Returns NULL if EOF reached
*/
static char	*read_heredoc_line(t_shell *shell)
{
	char	*line;
	(void)shell;
	line = readline("> ");			// prompt for heredoc line
	if (!line)
		return (NULL);			// EOF
	return (line);				// return user input
}

/* Read multiple lines for a heredoc until delimiter is reached
** - Uses arena for memory management
** - Returns full heredoc content or NULL
*/
char	*read_heredoc(t_shell *shell, const char *delimiter)
{
	printf("in read heredoc %i \n", shell->exit_code);
	char	*line;			// current heredoc line
	char	*content;		// accumulated content
	size_t	cur_len;		// current total length

	if (!delimiter)
		return (NULL);		// safety check

	content = arena_alloc(&shell->arena, 1);	// start with empty string
	if (!content)
		return (NULL);		// allocation failed
	content[0] = '\0';		// initialize empty string
	cur_len = 0;			// no bytes yet

	while (true)
	{
		line = read_heredoc_line(shell);	// get line from user
		if (!line || ft_strcmp(line, delimiter) == 0)
			break;			// reached delimiter or EOF

		content = append_heredoc_line(content, &cur_len, line, &shell->arena);
		if (!content)
			return (NULL);	// realloc failed
	}
	return (content);			// return full heredoc in arena
}

/* Unified function to handle heredoc input
** - Returns pointer to heredoc content in arena
*/
char	*handle_heredoc(t_cmd *cmd, t_shell *shell, const char *delimiter)
{
	(void)cmd;			// unused here but allows future extensions
	if (!delimiter)
		return (NULL);		// safety check
	return (read_heredoc(shell, delimiter)); // delegate actual reading
}
