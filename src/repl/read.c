#include "minishell.h"

/* Read a single line of input from user and duplicate into arena
** - Adds non-empty lines to history
** - Returns pointer in arena memory
*/
char	*read_input(t_arena **arena)
{
	char	*line;				// line from readline

	line = readline("minishell$ ");	// display prompt and read input
	if (!line)
		return (NULL);			// Ctrl-D returns NULL
	if (*line)
		add_history(line);		// store non-empty lines in history
	return (arena_strdup(arena, line));	// duplicate line into arena
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
static char	*read_heredoc_line(void)
{
	char	*line;

	line = readline("> ");			// prompt for heredoc line
	if (!line)
		return (NULL);			// EOF
	return (line);				// return user input
}

/* Read multiple lines for a heredoc until delimiter is reached
** - Uses arena for memory management
** - Returns full heredoc content or NULL
*/
char	*read_heredoc(t_arena **arena, const char *delimiter)
{
	char	*line;			// current heredoc line
	char	*content;		// accumulated content
	size_t	cur_len;		// current total length

	if (!arena || !delimiter)
		return (NULL);		// safety check

	content = arena_alloc(arena, 1);	// start with empty string
	if (!content)
		return (NULL);		// allocation failed
	content[0] = '\0';		// initialize empty string
	cur_len = 0;			// no bytes yet

	while (true)
	{
		line = read_heredoc_line();	// get line from user
		if (!line || ft_strcmp(line, delimiter) == 0)
			break;			// reached delimiter or EOF

		content = append_heredoc_line(content, &cur_len, line, arena);
		if (!content)
			return (NULL);	// realloc failed
	}
	return (content);			// return full heredoc in arena
}

/* Unified function to handle heredoc input
** - Returns pointer to heredoc content in arena
*/
char	*handle_heredoc(t_cmd *cmd, t_arena **arena, const char *delimiter)
{
	(void)cmd;			// unused here but allows future extensions
	if (!delimiter || !arena)
		return (NULL);		// safety check
	return (read_heredoc(arena, delimiter)); // delegate actual reading
}
#include "minishell.h"

/* Read a single line of input from user and duplicate into arena
 * - Adds non-empty lines to history
 * - Returns pointer in arena memory
 */
// static int checkforspacenblank(char *str)
// {
//     int i = 0;
//     while (str[i])
//     {
//         if (!(isblank(str[i]))) //
//             return 0;
//         i++;
//     }
//     return (1);
// }
// char *read_input(t_arena **arena)
// {
//     char *line;                                 // line from readline

//         line = readline("minishell$ ");             // display prompt and read input
//         if (!line)
//             return (NULL);                         // Ctrl-D returns NULL
//         // if (checkforspacenblank(line))
//         //     free(line);
//         if (*line)
//             add_history(line);                      // store non-empty lines in history
//         char *dup = arena_strdup(arena, line);       // duplicate line into arena
//         free(line);
//         return(dup);
// }

/* Read multiple lines for a heredoc until delimiter is reached
 * - Each line duplicated into arena
 * - Returns heredoc content or NULL if EOF
 */
// char *read_heredoc(t_arena **arena, const char *delimiter)
// {
//     char    *line;                               // current heredoc line
//     char    *content;                            // accumulated content
//     size_t  len;                                 // length of current line
//     size_t  cur_len;                             // current total length

//     content = arena_alloc(arena, 1);             // start with empty string
//     if (!content)
//         return (NULL);                            // allocation failed
//     content[0] = '\0';                           // initialize empty string
//     cur_len = 0;                                 // no bytes yet

//     while (true)
//     {
//         line = readline("> ");                   // prompt for heredoc line
//         if (!line || ft_strcmp(line, delimiter) == 0)
//             break;                               // reached delimiter or EOF

//         len = ft_strlen(line);                      // length of new line
//         content = arena_realloc(arena, content, cur_len, cur_len + len + 2); // +\n+\0
//         if (!content)
//             return (NULL);                       // realloc failed
//         ft_memcpy(content + cur_len, line, len);    // append line bytes
//         cur_len += len;                          // advance cursor
//         content[cur_len++] = '\n';               // append newline
//         content[cur_len] = '\0';                 // null terminate
//     }
//     return (content);                            // return full heredoc in arena
// }
