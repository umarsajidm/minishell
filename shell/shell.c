#include "minishell.h"

// maybe we can have a func that assigns the exit status and print a string
int init_shell(char **envp)
{
    t_arena arena; 
    shell.running = true;
    arena = init_arena(BUFFER_SIZE_FOR_ARENA);
    if (!arena)
    {
        ft_putstr_fd("minishell: failed to initialize memory arena\n");
        return (1);
    }
    shell.env = init_env(envp);
    if (!shell.env)
    {
        ft_putstr_fd("minishell: failed to initialize the enviroment\n", 2);
        free_arena(&arena);
        return (1);
    }
    return (0);
}