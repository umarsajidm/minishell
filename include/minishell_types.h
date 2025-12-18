#ifndef MINISHELL_TYPES_H
# define MINISHELL_TYPES_H

/* ===========================
** Standard types
** =========================== */
# include <stddef.h>  /* size_t */
# include <stdbool.h> /* bool */

//execution's fd
typedef struct s_fd
{
	int	fd[2];
	int	prev_fd;
	int	in_fd;
	int	out_fd;
}	t_fd;

typedef struct s_exec
{
    t_fd    *fd;
    pid_t   pid;
    char    *path_to_exec;
    char    **envp;
	int 	exit_flag;
}   t_exec;

/* ===========================
** Arena Memory Types
** =========================== */
typedef struct s_arena
{
	void			*memory_block; /* start of memory block */
	size_t			buffer;        /* total size of memory block */
	size_t			offset;        /* current offset in block */
	struct s_arena	*next;         /* next arena chunk */
}	t_arena;

# define GROWTH_FACTOR 2

/* ===========================
** Forward declarations
** =========================== */
typedef struct s_list	t_list;

/* ===========================
** Environment / Shell
** =========================== */
typedef struct s_env
{
	char			*key;   /* environment variable key */
	char			*value; /* environment variable value */
	struct s_env	*next;  /* next env node */
}	t_env;

typedef enum e_exec_flow
{
	FLOW_OK = 0,
	FLOW_EXIT
}	t_exec_flow;

typedef struct s_shell
{
	t_env	*env;        /* environment variables linked list */
	t_arena	*arena;      /* arena memory */
	int		exit_code;   /* last exit code */
	bool	running;     /* shell running flag */
	t_exec	*exec;
	t_exec_flow exit_flow;
	bool should_print_exit_message;
}	t_shell;

/* ===========================
** Token types
** =========================== */
typedef enum e_token_type
{
	T_WORD,        /* normal word */
	T_OPERATOR,    /* |  >< << >> */
	T_QUOTE,       /* quoted string */
	T_HEREDOC      /* heredoc delimiter token */
}	t_token_type;

typedef struct s_token
{
	char			*token;   /* token string */
	t_token_type	type;     /* token type */
	struct s_token	*next;    /* next token */
}	t_token;

/* ===========================
** Parser structures
** =========================== */
typedef enum e_redir_type
{
	R_INPUT,
	R_OUTPUT,
	R_APPEND,
	R_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;    /* type of redirection */
	char			*target; /* file or heredoc target */
	struct s_redir	*next;   /* next redirection */
}	t_redir;

typedef struct s_cmd
{
	char			**argv;       // command arguments
	t_redir			*redirs;      // redirection list
	struct s_cmd	*next;        // next command in pipeline
	char			*heredoc;     // content of heredoc, if any
	char			*unexpanded_cmd;
}	t_cmd;

/* ===========================
** Expansion helper types
** =========================== */
typedef struct s_index
{
	char	*str;   /* string being processed */
	int		i;      /* current index */
	int		j;      /* last processed index */
}	t_index;

/* Node for expansion key/value lookup */
typedef struct s_exp
{
	char			*key;    /* key */
	char			*value;  /* value */
	struct s_exp	*next;   /* next node */
}	t_exp;

/* Internal State struct for expansion parsing */
typedef struct s_state
{
	char	*res;       /* Result string */
	size_t	len;        /* Current length */
	size_t	i;          /* Current index in input */
	int		quote;      /* 0=None, 1=Single, 2=Double */
	t_shell	*shell;     /* Shell access */
	t_arena	**arena;    /* Memory arena */
}	t_state;


/* ===========================
** Global Variables
** =========================== */
extern volatile sig_atomic_t	g_signal; /* last received signal */

#endif /* MINISHELL_TYPES_H */