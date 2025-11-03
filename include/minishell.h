<<<<<<< HEAD
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

=======
>>>>>>> main
#ifndef MINISHELL_H
# define MINISHELL_H

/* ===========================
**        Standard Libraries
** =========================== */
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
<<<<<<< HEAD
// # include <sys/wait.h>
=======
>>>>>>> main
# include <fcntl.h>
# include <dirent.h>
# include <sys/stat.h>
# include <string.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

/* ===========================
**        Libft (types like t_list)
** =========================== */
# include "libft.h"

/* ===========================
**        Minishell Types
** =========================== */
# include "minishell_types.h"

typedef struct s_arena {
	void	*memory_block; // start of memory/pointer to the start of memory block
	size_t	buffer;	// total size of memory_block
	size_t	offset; //position from the starting point of memory_block
	struct	s_arena	*next;
}  				t_arena;

#define GROWTH_FACTOR 2

/* ===========================
**        Arena core functions
** =========================== */
void	*arena_alloc(t_arena **arena, size_t size);
void	*arena_realloc(t_arena **arena, void *old_ptr, size_t old_size, size_t new_size);
t_arena	*init_arena(size_t size);
t_arena	*new_bigger_arena(t_arena *current_arena, size_t size);
void	free_arena(t_arena **arena);
char	*arena_strdup(t_arena **arena, const char *s);

/* ===========================
**            Main
** =========================== */
int		main(int argc, char **argv, char **envp);

/* ===========================
**            REPL
** =========================== */
<<<<<<< HEAD
void	repl_loop(t_shell *shell, char **env);
char	*read_input(void);
char	*read_heredoc(const char *delimiter);
=======
void	repl_loop(t_shell *shell, t_arena **arena);
char	*read_input(t_arena **arena);
char	*read_heredoc(t_arena **arena, const char *delimiter);
>>>>>>> main

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
t_list	*tokenize(char *input, t_arena **arena);
int		skip_spaces(char *s, int i);
int		is_operator_char(char c);
char	*dup_word(t_arena **arena, const char *str, int start, int end);
int		handle_operator(char *s, int i, t_list **tokens, t_arena **arena);
int		handle_quote(char *s, int i, t_list **tokens, t_arena **arena);
int		handle_word(char *s, int i, t_list **tokens, t_arena **arena);
void	free_tokens(t_list **tokens); /* optional, mostly for debug */

/* ===========================
**          Parser
** =========================== */
t_cmd	*parse_tokens(t_list *tokens, t_arena **arena);
void	expand_variables(t_list *tokens, t_shell *shell);

/* parser helpers (arena-based) */
t_cmd	*create_cmd_node(t_arena **arena);
int		add_word_to_argv(t_cmd *cmd, const char *word, t_arena **arena);
int		add_redirection(t_cmd *cmd, t_redir_type type, const char *target, t_arena **arena);

/* parser token helpers */
int		is_pipe_token(const char *tok);
int		is_redir_token(const char *tok);
t_redir_type	get_redir_type(const char *tok);

/* ===========================
**        Executor
** =========================== */
<<<<<<< HEAD
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
=======
void	execute_command(t_shell *shell, t_cmd *cmds);
int		exec_single_cmd(t_shell *shell, t_cmd *cmd);
char	*find_executable(char *cmd, t_env *env);
int		exec_builtins(t_shell *shell, t_cmd *cmd);
>>>>>>> main

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

/* ===========================
**        Debug
** =========================== */
void	dbg_print_tokens(t_list *tokens);
void	dbg_print_cmds(t_cmd *cmds);
void	dbg_print_exit_code(int exit_code);

#endif /* MINISHELL_H */
