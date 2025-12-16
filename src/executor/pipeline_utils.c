#include "minishell.h"

static int write_heredoc_to_pipe(int *in_fd, char *content)
{
    int fd[2];

    if (pipe(fd) == -1)
    {
        perror("pipe");
        exit(1);
    }
    if (content)
    	ft_putstr_fd(content, fd[1]); // Write the stored content!

    close(fd[1]); // Close write end so child sees EOF

    if (*in_fd >= 0)
        close(*in_fd);
    *in_fd = fd[0]; // Set read end as input
	return (0);
}
static int	applying_input_redir(t_redir *r, int *in_fd)
{
	if (*in_fd >= 0)
		close(*in_fd);
	*in_fd = open(r->target, O_RDONLY);
	if (*in_fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(r->target, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		return (1);
	}
	return (0);
}

static int	applying_outside_redir(t_redir *r, int *out_fd)
{
	int	new_fd;

	if (r->type == R_OUTPUT)
		new_fd = open(r->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		new_fd = open(r->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (new_fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(r->target, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		return (1);
	}
	if (*out_fd >= 0)
		close(*out_fd);
	*out_fd = new_fd;
	return (0);
}

int		applying_redir(t_cmd *cmd, int *in_fd, int *out_fd)
{
	t_redir	*r;

	r = cmd->redirs;

	while(r)
	{
		if (r->target)
		{
			if (r->type == R_INPUT)
			{
				if (applying_input_redir(r, in_fd) == 1)
					return (1);
			}
			if (r->type == R_OUTPUT || r->type == R_APPEND)
			{
				if (applying_outside_redir(r, out_fd) == 1)
					return (1);
			}
			if (r->type == R_HEREDOC)
			{
				if (write_heredoc_to_pipe(in_fd, cmd->heredoc) == 1)
					return (1);
			}
		}
		r = r->next;
	}
	return (0);
}
