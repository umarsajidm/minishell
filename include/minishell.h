/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@hive.student.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 17:35:10 by achowdhu          #+#    #+#             */
/*   Updated: 2025/10/31 10:36:44 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ===========================
**        Standard Libraries
** =========================== */
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
// # include <sys/wait.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/stat.h>
# include <string.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

/* ===========================
**        Libft
** =========================== */
# include "libft.h"

/* ===========================
**        Structures
** =========================== */
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	t_env	*env;
	int		exit_code;
	bool	running;
}	t_shell;

typedef struct s_arena {
	void	*memory_block; // start of memory/pointer to the start of memory block
	size_t	buffer;	// total size of memory_block
	size_t	offset; //position from the starting point of memory_block
	struct	s_arena	*next;
}  				t_arena;

#define GROWTH_FACTOR 2

/* ===========================
**        Global Variables
** =========================== */
extern int	g_signal;

/* ===========================
**        Main
** =========================== */
int	main(int argc, char **argv, char **envp);

/* ===========================
**            REPL
** =========================== */
void	repl_loop(t_shell *shell, char **env);
char	*read_input(void);
char	*read_heredoc(const char *delimiter);

/* ===========================
**        Signals
** =========================== */
void	setup_signals(void);
void	handle_sigint(int sig);
void	handle_sigquit(int sig);

/* ===========================
**           Utils
** =========================== */
void	shell_error(const char *msg);
t_env	*init_env(char **envp);
void	free_env(t_env *env);
char	*get_env_value(t_env *env, const char *key);
//arena_malloc
void	*allocation(t_arena **arena, size_t size);
t_arena *init_arena(size_t size);
t_arena	*new_bigger_arena(t_arena *current_arena, size_t size);
void	free_arena(t_arena **arena);

/* ===========================
**         Tokenizer
** =========================== */
t_list	*tokenize(char *input);
int		skip_spaces(char *s, int i);
int		is_operator_char(char c);
char	*dup_word(const char *str, int start, int end);
int		handle_operator(char *s, int i, t_list **tokens);
int		handle_quote(char *s, int i, t_list **tokens);
int		handle_word(char *s, int i, t_list **tokens);

/* ===========================
**          Parser
** =========================== */
void	parse_tokens(t_list *tokens);
void	expand_variables(t_list *tokens, t_shell *shell);

/* ===========================
**        Executor
** =========================== */
//void	execute_command(t_shell *shell, t_list *cmd);
//execution.c
void	child_process(t_list *tokens, char **env);
//execution_utilities
void	strerrornexit(void);
void	freearray(char **arr);
void	freestrnarrexit(char **arr, char *str, int i);
void	freeerror(char **arr);
void	freeall(char **arr, char *str, char *cmd);
void	commandnotfound(char **arr);

/* ===========================
**        Builtins
** =========================== */
int		builtin_echo(t_list *args);
int		builtin_cd(t_shell *shell, t_list *args);
int		builtin_pwd(void);
int		builtin_export(t_shell *shell, t_list *args);
int		builtin_unset(t_shell *shell, t_list *args);
int		builtin_env(t_shell *shell);
int		builtin_exit(t_shell *shell, t_list *args);

#endif
