#include "minishell.h"

/* 
 * Main REPL loop
 * - Reads input from user
 * - Tokenizes input using arena
 * - Expands variables (currently disabled)
 * - Parses commands using arena
 * - Executes commands (currently disabled)
 * - Loops until shell->running == false
 */
void	repl_loop(t_shell *shell, t_arena **arena)
{
	char    *input;     // input line (arena memory)
	t_list  *tokens;    // token list (arena nodes)
	t_cmd   *commands;  // parsed commands (arena nodes)

	while (shell->running)
	{
		input = read_input(arena);               // read user input into arena
		if (!input)
			break;                               // Ctrl-D exits

		tokens = tokenize(input, arena);         // tokenize input using arena
		dbg_print_tokens(tokens);                // debug tokens

		/* variable expansion (disabled for now) */
		// expand_variables(tokens, shell);

		/* parse tokens into commands */
		commands = parse_tokens(tokens, arena);  // build command structures
		if (!commands && tokens)                 // parse failed (syntax or alloc)
		{
			// parsing error already printed by parse_tokens()
			(void)tokens;
			continue;                             // skip execution
		}
		dbg_print_cmds(commands);                // show parsed commands

		/* execute commands (disabled for now) */
		// execute_command(shell, commands);
		pipeline(commands, shell);

		// dbg_print_exit_code(shell->exit_code);   // debug exit code

		/* avoid unused variable warnings while features are stubbed */
		(void)tokens;
		(void)commands;
	}
}
