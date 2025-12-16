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
	// dbg_print_cmds(commands);
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
            break ;
        }
        if (*input)
            add_history(input);

        process_line(shell, arena, input);
        arena_clear(arena);
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
    }
}
