#include "minishell.h"

// maybe we can have a func that assigns the exit status and print a string
// int init_shell(t_shell *shell, char **envp)
// {
//     shell->running = true;
//     shell->env = init_env(envp);
//     if (!shell->env)
//     {
//         ft_putstr_fd("minishell: failed to initialize the enviroment\n", 2);
//         return (1);
//     }

//     return (0);
// }