#include "minishell.h"

/*
 * Main REPL loop
 * - Reads input from user
 * - Tokenizes input using arena
 * - Expands variables in commands
 * - Parses commands using arena
 * - Executes commands (currently disabled)
 * - Loops until shell->running == false
 */
void	repl_loop(t_shell *shell, t_arena **arena)
{
	char	*input;     // input line (arena memory)
	t_list	*tokens;    // token list (arena nodes)
	t_cmd	*commands;  // parsed commands (arena nodes)
	int		res;        // expansion result

	while (shell->running)
	{
		input = read_input(arena);               // read user input into arena
		if (!input)
			break;                               // Ctrl-D exits

		/* if the line is empty or only whitespace, skip processing */
		if (is_blank_line(input))
		{
			arena_clear(arena);
			continue;
		}

		tokens = tokenize(input, arena);         // tokenize input using arena
		// dbg_print_tokens(tokens);                // debug tokens

		/* parse tokens into commands */
		commands = parse_tokens(tokens, shell, arena);
		if (!commands && tokens)                 // parse failed (syntax or alloc)
		{
			(void)tokens;                        // parsing error already printed
			arena_clear(arena);
			continue;
		}
		// dbg_print_cmds(commands);                // show parsed commands

		/* variable expansion */
		res = expand_command_argv(commands, shell, arena);
		if (res == 0)                            // allocation failure during expansion
		{
			ft_printf("minishell: parse error: alloc fail\n"); // formatted error
			arena_clear(arena);
			continue;
		}

		/* debug expanded argv (42 Norminette safe) */
		// dbg_print_expanded_argv(commands);

		//test_builtin(commands, shell);
		if (commands->argv != NULL)
			execution_pipeline(commands, shell);
		arena_clear(arena);

		// dbg_print_exit_code(shell->exit_code);   // debug exit code

		/* avoid unused variable warnings while features are stubbed */
		(void)tokens;
		(void)commands;
	}
}
