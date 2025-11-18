#include "execution.h"

typedef struct redir_pipeline
{
    int	in_fd;
    int	out_fd;
	t_redir	*redir;
}	redir_pipeline;

static void	applying_input_redir(t_redir *r, int *in_fd)
{
	if (*in_fd >= 0)
		close(*in_fd);
	*in_fd = open(r->target, O_RDONLY);
	if (*in_fd < 0)
		perror("opening of input redirection");
}

static void	applying_outside_redir(t_redir *r, int *out_fd)
{
	if (*out_fd >= 0)
		close(*out_fd);
	*out_fd = open(r->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*out_fd < 0)
		perror("opening of out redirection or heredoc");
}

void	applying_redir(t_redir *r, int *in_fd, int *out_fd)
{
	while(r)
	{
		if (r->target)
		{
			if (r->type == R_INPUT)
				applying_input_redir(r, in_fd);
			else if (r->type == R_OUTPUT)
				applying_outside_redir(r, out_fd);
		}
		r = r->next;
	}	
}
