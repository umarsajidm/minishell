#include "minishell.h"

typedef struct s_env
{
	char			*key;   /* environment variable key */
	char			*value; /* environment variable value */
	struct s_env	*next;  /* next env node */
}	t_env;

typedef struct s_shell
{
	t_env	*env;        /* environment variables linked list */
	int		exit_code;   /* last exit code */
	bool	running;     /* shell running flag */
}	t_shell;


static size_t sizeoflist(t_env *env);


//making the envp from linked list of env for execve func
char **envp(t_shell **shell)
{
    t_env *env;
    int i;
    env = shell->env;
    i = 0;
    char **envp = ft_calloc(sizeoflist(shell->env) + 1, sizeof(char *));
    if (!envp)
        return NULL; //update with exit status and error message
    while (env)
    {   
        envp[i] = ft_strdup(copyfromlisttoarr(env));
        if (env->next)
            i++;
        env = env->next;
    }
    envp[i] = '\0';
    return (envp);
}

static size_t sizeoflist(t_env *env)
{
    t_env   *cur;
    size_t  i;

    i = 0;
    cur = env;
    while(cur)
    {
        if (cur->value)
            i++;
        cur = cur->next;
    }
    return (1);
}

static char *copyfromlisttoarr(t_env *env)
{
    size_t keylen;
    size_t valuelen;
    char *dest;

    keylen = ft_strlen(env->key);
    valuelen = ft_strlen(env->value);
    dest = ft_calloc(keylen + valuelen + 2);
    ft_memcpy(dest, env->key, keylen);
    dest[keylen] = '=';
    ft_memcpy(dest + keylen + 1, env->value, valuelen);
    dest[valuelen] = '\0';
    return (dest);
}

