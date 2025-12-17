#include "minishell.h"

static void process_line(t_shell *shell, t_arena **arena, char *input)
{
    t_list  *tokens;
    t_cmd   *commands;

    if (is_blank_line(input))
        return ;

    tokens = tokenize(input, arena);
    if (!tokens)
    {
        shell->exit_code = 2;
        return ;
    }

    commands = parse_tokens(tokens, shell, arena);
    if (!commands)
    {
		setup_parent_signals();
        shell->exit_code = 2;
        return ;
    }

    if (commands && (commands->argv || commands->unexpanded_cmd))
        main_pipeline(shell, commands);
}

void    repl_loop(t_shell *shell, t_arena **arena)
{
    char    *input;

    while (shell->running)
    {
        input = read_input(arena);
        if (!input)
        {
            ft_exit2(NULL, shell); // Pass false for is_child_process
        }
		if (g_signal != 0)
		{
			shell->exit_code = 128 + g_signal;
			g_signal = 0;
		}
        if (input && *input) // Only add to history if input is not NULL and not empty
            add_history(input);
		else if (!input && shell->exit_flow == FLOW_EXIT) // Handle Ctrl-D or non-interactive EOF
		{
			// Conditional print moved here
			if (shell->should_print_exit_message && isatty(STDIN_FILENO))
				ft_putstr_fd("exit\n", STDOUT_FILENO);
			shell->should_print_exit_message = false; // Reset
			shell->running = false;
			shell->exit_flow = FLOW_OK; // Reset for next potential run, though shell will exit
			break; // Exit loop immediately
		}

		if (input) // Only process line if input is not NULL
			process_line(shell, arena, input);
        arena_clear(arena);

		// Check exit flow after processing the line
		if (shell->exit_flow == FLOW_EXIT)
		{
			if (shell->should_print_exit_message && isatty(STDIN_FILENO))
				ft_putstr_fd("exit\n", STDOUT_FILENO);
			shell->should_print_exit_message = false; // Reset
			shell->running = false;
			shell->exit_flow = FLOW_OK; // Reset for next potential run, though shell will exit
		}
    }
}

void    non_interactive_loop(t_shell *shell, t_arena **arena)
{
    char    *input;

    while (shell->running)
    {
        input = read_input(arena);
        if (!input)
            break;

        if (input)
            process_line(shell, arena, input);

        arena_clear(arena);
		// Check exit flow after processing the line
		if (shell->exit_flow == FLOW_EXIT)
		{
			if (shell->should_print_exit_message) // isatty check not needed here, non-interactive
				ft_putstr_fd("exit\n", STDOUT_FILENO);
			shell->should_print_exit_message = false; // Reset
			shell->running = false;
			shell->exit_flow = FLOW_OK; // Reset for next potential run, though shell will exit
		}
    }
}
